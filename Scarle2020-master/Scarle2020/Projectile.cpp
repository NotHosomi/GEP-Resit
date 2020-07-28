#include "pch.h"
#include "Projectile.h"
#include "GameData.h"

Projectile::Projectile(ID3D11Device* _GD, string texture, Vector2 _dimensions, float _weight, float _elasticity, bool _explode_on_contact)
	: ImageGO2D(texture, _GD),
	PhysCmp(_dimensions, _weight, _elasticity)
{
	explode_on_contact = _explode_on_contact;
}

void Projectile::Tick(GameData* _GD)
{
	bool hit = PhysCmp.move(_GD->m_dt, _GD->p_World, m_pos);
	if (explode_on_contact && hit)
	{
		explode(_GD);
	}
}

void Projectile::explode(GameData* _GD)
{
}
