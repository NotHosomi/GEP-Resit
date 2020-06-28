#include "pch.h"
#include "Worms.h"

#include <iostream>

void Worms::Init(string _fileName, ID3D11Device* _GD)
{
	//sprite component set up
	//SetSprite(_fileName, _GD);
	//SetPos(m_spawnPos);
	//SetScale(m_SCALE);
	//physics component
	//AddPhysicsComponent();
	//physicsComponent()->setXSpeed(200);
	//physicsComponent()->setYSpeed(5000);

	////AddCollisionComponent();
	////when ever you draw a worm pass the rect in too
	//RECT rect;
	//rect.left = GetPos().x;
	//rect.top = GetPos().y;
	//rect.right = rect.left + m_width;
	//rect.bottom = rect.top + m_height;
	////m_collisionComp->Init(rect);

	//m_width *= m_SCALE;
	//m_height *= m_SCALE;
}

/*
move left and right first with out key input

*/
void Worms::Tick(GameData* _GD) 
{
	//Movement(1, true); // need to use delta timewhere 1 is passed in
}

void Worms::setHealth(float _health)
{
	_health = player_health;
}

float Worms::getHealth()
{
	return player_health;
}
