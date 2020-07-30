#include "pch.h"
#include "Grenade.h"
#include "GameData.h"

Grenade::Grenade(ID3D11Device* _GD, Vector2 position, Vector2 velocity, float weight, float elasticity) :
	Projectile(_GD, "projectile2", position, velocity, Vector2(GREN_DIMS, GREN_DIMS), weight, elasticity)
{
	exp_radius = GREN_EXP_RADIUS;
	exp_damage = GREN_EXP_DMG;
	fuse = GREN_FUSE;
}

void Grenade::Tick(GameData* _GD)
{
	// TODO: Fix grenade disappearing on collision
	PhysCmp.move(_GD->m_dt, _GD->p_World, m_pos);

	fuse_tmr += _GD->m_dt;
	if (fuse_tmr > fuse)
	{
		explode(_GD);
	}
}