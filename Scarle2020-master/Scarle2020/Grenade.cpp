#include "pch.h"
#include "Grenade.h"

//Grenade::Grenade(ID3D11Device* _GD, Vector2 _pos, Vector2 initial_vel)
//{
//	Setup(_GD, _pos, initial_vel, "grenade");
//}

void Grenade::Tick(GameData* _GD)
{
	//apply gravity
	//if hit ground bounce
	//after set time explode & delete self?

	elapsed_time += _GD->m_dt;

	float y_vel = m_physicsComp->getVelY();

	if (elapsed_time >= explode_time)
	{
		//m_physicsComp->setVelocity(0, 0);
		//Explode();
		physicsComponent()->setBounce(false);
	}
	else if (m_pos.y >= 500)
	{
		//y_vel *= -0.5f;
		//m_physicsComp->setYVector(y_vel);
	}
	else
	{
		//y_vel += weight * _GD->m_dt;
		//m_physicsComp->setYVector(y_vel);
	}

	m_physicsComp->move(_GD->m_dt, m_pos);
	//updateAngle();
	m_physicsComp->rotate(m_rotation, XM_PI, _GD->m_dt);
}