#include "pch.h"
#include "Grenade.h"
#include "GameData.h"

Grenade::Grenade(ID3D11Device* _GD, const Vector2& location, const Vector2& velocity) :
	ImageGO2D("projectile2", _GD),
	PhysCmp(Vector2(GREN_DIMS, GREN_DIMS), GREN_WEIGHT, GREN_ELASTICITY)
{
	m_pos = location;
	PhysCmp.setVel(velocity);
}

void Grenade::Tick(GameData* _GD)
{
	PhysCmp.move(_GD->m_dt, _GD->p_world, m_pos);

	fuse_tmr += _GD->m_dt;
	if (fuse_tmr > GREN_FUSE)
	{
		// TODO: Explode!
	}
}