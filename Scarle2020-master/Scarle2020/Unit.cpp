#include "pch.h"
#include "Unit.h"
#include "GameData.h"

Unit::Unit(ID3D11Device* _GD, const Vector2& location, int team) :
	ImageGO2D("ally", _GD),
	PhysCmp(Vector2(UNIT_WIDTH, UNIT_HEIGHT), UNIT_WEIGHT, UNIT_ELASTICITY)
{
	m_pos = location;
	team_id = team;
}

//Unit::Unit(const Unit& other) : ImageGO2D(other),
//	PhysCmp(other.PhysCmp)
//{
//}

Unit::Unit(Unit&& other) noexcept : ImageGO2D(std::move(other)), 
	PhysCmp(other.PhysCmp)
{
	awake = other.awake;
	alive = other.alive;
	health = other.health;
	accumulated_damage = other.accumulated_damage;
	team_id = other.team_id;
	facing_right = other.facing_right;
	jump_timer = other.jump_timer;
}

void Unit::Tick(GameData* _GD)
{
	if (awake)
	{
		playerMove(_GD);
	}
	PhysCmp.move(_GD->m_dt, _GD->p_world, m_pos);
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

void Unit::addDamage(float amount)
{
	accumulated_damage += amount;
}

void Unit::applyDamages()
{
	health -= accumulated_damage;
	accumulated_damage = 0;
	if (health <= 0)
	{
		// TODO: EXPLODE!
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
