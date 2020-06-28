#include "pch.h"
#include "HomingMissile.h"

HomingMissile::HomingMissile(ID3D11Device* _GD, Vector2 _pos, Vector2 initial_vel, float _power, Vector2 _target) : Missile{ _GD, _pos, initial_vel, 500 }
{
	target_pos = _target;		
	m_width = 22;
	m_height = 12;
}

void HomingMissile::Tick(GameData* _GD)
{
	elapsed_time += _GD->m_dt;

	if (elapsed_time >= homing_time && elapsed_time <= homing_time + delay_time)
	{
		if (!homing)
		{
			m_physicsComp->setGravity(false);
			m_physicsComp->setVelocity(target_pos - m_pos);
			m_physicsComp->rotate(m_rotation, XM_PI, _GD->m_dt);
			homing = true;
		}
		//updateAngle();
		m_physicsComp->move(_GD->m_dt, m_pos);
	}
	else
	{
		//m_physicsComp->setGravity(true);
		Missile::Tick(_GD);
	}
}