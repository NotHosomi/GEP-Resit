#include "pch.h"
#include "Projectile.h"

Projectile::Projectile(ID3D11Device* _GD, Vector2 _pos, Vector2 initial_vel, std::string sprite, float _speed, float _weight)
{
	_speed = 200;
	weight = 1;
	SetSprite(sprite, _GD);
	m_pos.x = _pos.x;
	m_pos.y = _pos.y;
	AddPhysicsComponent(m_pos);
	m_physicsComp->setVelocity(initial_vel);
	m_physicsComp->setXSpeed(_speed);
	m_physicsComp->setYSpeed(_speed);
	weight = _weight;
	m_physicsComp->setPos(m_pos);
	//velocity = initial_vel;

	//liams code
	AddCollisionComponent();//doesnt a projectile need a collision component????
	m_width = 100; //set to width of image
	m_height = 100; //set to height of image
	RECT rect;
	rect.left = m_pos.x;
	rect.top = m_pos.y;
	rect.right = m_pos.x + m_width;
	rect.bottom = m_pos.y + m_height;
	m_collisionComp->Init(rect);
}

void Projectile::Tick(GameData* _GD)
{
}

void Projectile::Move(GameData* _GD)
{
	//m_pos += m_physicsComp.get * _GD->m_dt;
}

void Projectile::Explode()
{
	/*Destroy Terrain arround and delete self
	*/
}

void Projectile::updateAngle()
{
	float angle = atan(m_physicsComp->getYVector() / m_physicsComp->getXVector());


	if (m_physicsComp->getXVector() < 0)
	{
		angle += (XM_PI);
	}

	m_rotation = angle;
}