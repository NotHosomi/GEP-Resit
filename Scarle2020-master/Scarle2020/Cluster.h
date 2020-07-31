#pragma once
#include "grenade.h"
class Cluster :
	public Grenade
{
public:
	Cluster(ID3D11Device* _GD, Vector2 position, Vector2 velocity);
	void explode(GameData* _GD) override;

private:
	static constexpr float CLUSTER_DIMS = 10;
	static constexpr float CLUSTER_EXP_RADIUS = 50;
	static constexpr float CLUSTER_EXP_DMG = 7;
	static constexpr float CLUSTER_FUSE = 3;
	static constexpr float CLUSTER_NUM_CHILDREN = 5;
};

