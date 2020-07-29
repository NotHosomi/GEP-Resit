#include "pch.h"
#include "Projectile.h"
#include "GameData.h"
#include "Explosion.h"

Projectile::Projectile(ID3D11Device* _GD, string texture, Vector2 position, Vector2 velocity,
	Vector2 _dimensions, float _weight, float _elasticity)
	: ImageGO2D(texture, _GD),
	PhysCmp(_dimensions, _weight, _elasticity)
{
	m_pos = position;
	PhysCmp.setVel(velocity);
}

void Projectile::Tick(GameData* _GD)
{
	bool hit = PhysCmp.move(_GD->m_dt, _GD->p_World, m_pos);
	if (hit)
	{
		explode(_GD);
	}
}

void Projectile::explode(GameData* _GD)
{
	GameObject2D* explosion = new Explosion(_GD->p_Device, m_pos, exp_radius, exp_damage);
	_GD->creation_list.emplace_back(explosion);
	_GD->deletion_list.emplace_back(this);
	_GD->m_Turn.setWaiting(false);
}

void Projectile::OOBCheck(GameData* _GD)
{
	bool OOB = false;
	// URGH! Hard coded bounds, disgusting!
	// TODO: pass the window resolution thru GameData
	if (m_pos.x + PhysCmp.getCollider().width / 2 < 0 ||
		m_pos.x - PhysCmp.getCollider().width / 2 > 1280 ||
		m_pos.x - PhysCmp.getCollider().height / 2 > 720)
	{
		_GD->deletion_list.emplace_back(this);
		_GD->m_Turn.setWaiting(false);
	}
	// Note: Intentionally permits objects to go above the screen, as gravity will bring them back down
}
