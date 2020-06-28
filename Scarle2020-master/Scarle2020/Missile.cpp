#include "pch.h"
#include "Missile.h"


//Missile::Missile(ID3D11Device* _GD , Vector2 _pos, Vector2 initial_vel)/
//{
//	Setup(_GD, _pos, initial_vel, "missile");
//}

void Missile::Tick(GameData* _GD)
{
	//apply gravity as moves
	//on collision explode and delete


	//float y_vect = m_physicsComp->getYVector() + 1 * _GD->m_dt;
	//m_physicsComp->setYVector(y_vect);



	m_physicsComp->rotate(m_rotation, XM_PI, _GD->m_dt);//add this to physics state machine
	m_physicsComp->move(_GD->m_dt, m_pos);
}