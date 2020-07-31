#pragma once
#include "grenade.h"
class ClusterChild :
	public Grenade
{
public:
	ClusterChild(ID3D11Device* _GD, Vector2 position, Vector2 velocity);
	void Tick(GameData* _GD) override;
	void Draw(DrawData2D* _DD) override;
	void explode(GameData* _GD) override;
private:
	static constexpr float CLUSTERCHILD_EXP_RADIUS = 20;
	static constexpr float CLUSTERCHILD_EXP_DMG = 7;
	bool hidden = false;
};

