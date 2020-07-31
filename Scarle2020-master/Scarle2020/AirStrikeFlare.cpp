#include "pch.h"
#include "AirStrikeFlare.h"
#include "GameData.h"
#include "Bullet.h"

AirStrikeFlare::AirStrikeFlare(ID3D11Device* _GD, Vector2 position, Vector2 velocity) :
	Projectile(_GD, "projectile", position, velocity, Vector2(FLARE_DIMS, FLARE_DIMS), FLARE_WEIGHT, FLARE_ELASTICITY)
{
	m_scale = Vector2(FLARE_DIMS, FLARE_DIMS);
	m_colour = FLARE_COLOUR;
}

void AirStrikeFlare::Tick(GameData* _GD)
{
	PhysCmp.move(_GD->m_dt, _GD->p_World, m_pos);

	if (strike_started)
	{
		strikeTick(_GD);
	}

	if (PhysCmp.isStill())
	{
		PhysCmp.setLocked(true);

	}

	OOBCheck(_GD);
}

void AirStrikeFlare::strikeTick(GameData* _GD)
{
	strike_timer += _GD->m_dt;
	if (strike_timer > STRIKE_GAP)
	{
		strike_timer = 0;
		if (strikes_occured < STRIKE_NUM)
		{
			strike(_GD);
		}
		else
		{
			// The strike has finished
			_GD->deletion_list.emplace_back(this);
			_GD->m_Turn.setWaiting(false);
		}
	}
}

void AirStrikeFlare::strike(GameData* _GD)
{
	Vector2 pos = m_pos;
	pos.y = -10;
	pos.x += STRIKE_GAP_DIST * STRIKE_NUM / 2;
	pos.x += STRIKE_GAP_DIST * strikes_occured;
	GameObject2D* new_strike = new Bullet(_GD->p_Device, pos, Vector2(), STRIKE_DMG, STRIKE_EXP_RADIUS);
	_GD->creation_list.emplace_back(new_strike);
}
