#include "pch.h"
#include "Cluster.h"
#include "GameData.h"
#include "Explosion.h"
#include "Rocket.h"
#include "ClusterChild.h"

Cluster::Cluster(ID3D11Device* _GD, Vector2 position, Vector2 velocity) :
	Grenade(_GD, position, velocity)
{
	exp_radius = CLUSTER_EXP_RADIUS;
	exp_damage = CLUSTER_EXP_DMG;
	fuse = CLUSTER_FUSE;
	// Darken the base grenade
	SetColour(Color(0.6, 0.6, 0.6));
}

void Cluster::explode(GameData* _GD)
{
	GameObject2D* explosion = new Explosion(_GD->p_Device, m_pos, exp_radius, exp_damage);
	_GD->creation_list.emplace_back(explosion);
	for (int i = 0; i < CLUSTER_NUM_CHILDREN; ++i)
	{
		Vector2 launch_dir = Vector2(0, 100);
		launch_dir.x = (i-2) * 10;
		GameObject2D* child = new ClusterChild(_GD->p_Device, m_pos, launch_dir);
		_GD->creation_list.emplace_back(child);
	}
	_GD->deletion_list.emplace_back(this);
}
