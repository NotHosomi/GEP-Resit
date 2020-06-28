#include "pch.h"
#include "Explosion.h"
#include "DestructionCircle.h"

Explosion::Explosion(ID3D11Device* _GD, Vector2 location, float radius, float damage)
{
	//DestructionCircle("", _GD, radius, location);
	m_pos = location;
	m_radius = radius;
	m_damage = damage;

	// TODO:
	// Find GOs in radius
	
	// Apply phys impulse

	// Apply damage
}

void Explosion::Tick(GameData* _GD)
{
	age += _GD->m_dt;
}
