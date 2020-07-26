#include "pch.h"
#include "Unit.h"
#include "GameData.h"

Unit::Unit(ID3D11Device* _GD, const Vector2& location) : 
	ImageGO2D("ally", _GD),
	PhysCmp(Vector2(UNIT_WIDTH, UNIT_HEIGHT), UNIT_WEIGHT, UNIT_ELASTICITY)
{
	m_pos = location;
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
	if (_GD->m_Input.checkKey(InputManager::IN_JUMP))
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
