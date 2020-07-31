#include "pch.h"
#include "ClusterChild.h"
#include "GameData.h"
#include "Explosion.h"
#include "DrawData2D.h"

ClusterChild::ClusterChild(ID3D11Device* _GD, Vector2 position, Vector2 velocity) :
	Grenade(_GD, position, velocity)
{
	exp_radius = CLUSTERCHILD_EXP_RADIUS;
	exp_damage = CLUSTERCHILD_EXP_DMG;
	// Darken the base grenade
	SetColour(Color(0.6, 0.6, 0.6));
	SetScale(Vector2(0.5, 0.5));
}

void ClusterChild::Tick(GameData* _GD)
{
	bool hit = PhysCmp.move(_GD->m_dt, _GD->p_World, m_pos);
	if (hit)
	{
		explode(_GD);
	}
	fuse_tmr += _GD->m_dt;
	if (fuse_tmr > fuse)
	{
		explode(_GD);
		_GD->deletion_list.emplace_back(this);
		_GD->m_Turn.setWaiting(false);
	}
}

void ClusterChild::Draw(DrawData2D* _DD)
{
	if (!hidden)
	{
		_DD->m_Sprites->Draw(m_pTextureRV, m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
	}
}

void ClusterChild::explode(GameData* _GD)
{
	// This mess with checking Hidden and not destroying itself after exploding is
	// so that the turn manager doesn't stop waiting after only the first child has exploded
	// Thus, they all wait until the full fuse has passed before ending the turn and destroying
	if (hidden)
	{
		return;
	}
	GameObject2D* explosion = new Explosion(_GD->p_Device, m_pos, exp_radius, exp_damage);
	_GD->creation_list.emplace_back(explosion);
	hidden = true;
}
