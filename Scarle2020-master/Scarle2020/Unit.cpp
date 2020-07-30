#include "pch.h"
#include "Unit.h"
#include "GameData.h"
#include "DrawData2D.h"
#include "TeamsManager.h"
#include "Explosion.h"

Unit::Unit(ID3D11Device* _GD, const Vector2& location, int team) :
	ImageGO2D("unit", _GD),
	PhysCmp(Vector2(UNIT_WIDTH, UNIT_HEIGHT), UNIT_WEIGHT, UNIT_ELASTICITY)
{
	m_pos = location;
	team_id = team;
	m_scale = Vector2(UNIT_WIDTH, UNIT_HEIGHT);
	hp_text.SetScale(UNIT_HP_BAR_SCALE);
	hp_text.SetColour(TeamsManager::colourPicker(team_id));
}

Unit::Unit(const Unit& other) : ImageGO2D(other),
	PhysCmp(other.PhysCmp),
	hp_text(other.hp_text)
{
	awake = other.awake;
	alive = other.alive;
	health = other.health;
	accumulated_damage = other.accumulated_damage;
	team_id = other.team_id;
	facing_right = other.facing_right;
}

Unit::Unit(Unit&& other) noexcept : ImageGO2D(std::move(other)), 
	PhysCmp(other.PhysCmp),
	hp_text(other.hp_text)
{
	awake = other.awake;
	alive = other.alive;
	health = other.health;
	accumulated_damage = other.accumulated_damage;
	team_id = other.team_id;
	facing_right = other.facing_right;
}

void Unit::Tick(GameData* _GD)
{
	if (awake)
	{
		playerMove(_GD);
	}
	PhysCmp.move(_GD->m_dt, _GD->p_World, m_pos);
	Vector2 hp_loc = m_pos;
#if 0
	hp_loc.y += UNIT_HP_BAR_OFFSET;
#else
	hp_loc.y += UNIT_HP_BAR_OFFSET;
	hp_loc.y -= UNIT_HEIGHT;
	hp_loc.x -= UNIT_WIDTH/2;
#endif
	hp_text.SetPos(hp_loc);
	OOBCheck(_GD);
}

void Unit::Draw(DrawData2D* _DD)
{
	if (alive)
	{
		_DD->m_Sprites->Draw(m_pTextureRV, m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
		hp_text.Draw(_DD);
	}
}

void Unit::setAwake(bool _awake)
{
	awake = _awake;
}

int Unit::getTeam()
{
	return team_id;
}

bool Unit::isAlive()
{
	return alive;
}

bool Unit::isFlipped()
{
	return !facing_right;
}

void Unit::addDamage(float amount)
{
	accumulated_damage += amount;
}

void Unit::applyDamages(GameData* _GD)
{
	health -= accumulated_damage;
	accumulated_damage = 0;
	hp_text.SetString(std::to_string(health));
	if (health <= 0 && alive)
	{
		die(_GD);
		explode(_GD);
	}
}

void Unit::playerMove(GameData* _GD)
{
	if (!PhysCmp.isGrounded())
	{
		// Air move
		if (_GD->m_Input.checkKey(InputManager::IN_RIGHT))
		{
			facing_right = true;
			if (PhysCmp.getVel().x < MV_MAXAIRCONTROL)
				PhysCmp.addXVel(MV_AIRACCELERATION);
		}
		if (_GD->m_Input.checkKey(InputManager::IN_LEFT))
		{
			facing_right = false;
			if(PhysCmp.getVel().x > -MV_MAXAIRCONTROL)
				PhysCmp.addXVel(-MV_AIRACCELERATION);
		}
		return;
	}

	// Walk move
	if (_GD->m_Input.checkKey(InputManager::IMP_JUMP))
	{
		PhysCmp.setGrounded(false);
		PhysCmp.addYVel(-MV_JUMPFORCE_V);
		PhysCmp.setXVel(facing_right ? MV_JUMPFORCE_H : -MV_JUMPFORCE_H);
	}
	if (_GD->m_Input.checkKey(InputManager::IN_RIGHT))
	{
		facing_right = true;
		PhysCmp.addXVel(MV_ACCELERATION);
	}
	if (_GD->m_Input.checkKey(InputManager::IN_LEFT))
	{
		facing_right = false;
		PhysCmp.addXVel(-MV_ACCELERATION);
	}
}

void Unit::OOBCheck(GameData* _GD)
{
	bool OOB = false;
	// URGH! Hard coded bounds, disgusting!
	// TODO: pass the window resolution thru GameData
	if (m_pos.x + PhysCmp.getCollider().width / 2 < 0 ||
		m_pos.x - PhysCmp.getCollider().width / 2 > 1280 ||
		m_pos.y - PhysCmp.getCollider().height / 2 > 720)
	{
		die(_GD);
	}
	// Note: Intentionally permits objects to go above the screen, as gravity will bring them back down
}

// Does not explode the unit. Call that separately
void Unit::die(GameData* _GD)
{
	alive = false;
	awake = false;
	// effectively turn off the PhysCmp
	PhysCmp.setLocked(true);
	PhysCmp.setVel(Vector2(0, 0));

	//check if the current unit was the one that just died
	if (_GD->m_Teams.getCurrentUnit() == this)
	{
		if (_GD->m_Turn.getState() == TurnManager::TurnState::TS_ACT)
		{
			_GD->m_Turn.nextStage(&_GD->m_Teams);
			_GD->m_Turn.nextStage(&_GD->m_Teams);
		}
		else if (_GD->m_Turn.getState() == TurnManager::TurnState::TS_FLEE)
		{
			_GD->m_Turn.nextStage(&_GD->m_Teams);
		}
	}
}

void Unit::explode(GameData* _GD)
{
	GameObject2D* explosion = new Explosion(_GD->p_Device, m_pos, DEATH_EXP_RADIUS, DEATH_EXP_DMG);
	_GD->creation_list.emplace_back(explosion);
}
