#pragma once

#include "Projectile.h"
#include "PhysicsComponent.h"

class Grenade : public Projectile
{
public:
	Grenade(ID3D11Device* _GD, Vector2 position, Vector2 velocity);

	void Tick(GameData* _GD) override;

	// debugging
	PhysicsComponent* getPhysCmp() { return &PhysCmp; };

private:
	// golden ratio 1 : 1.62
	static constexpr float GREN_DIMS = 10;
	static constexpr float GREN_WEIGHT = 300;
	static constexpr float GREN_ELASTICITY = 0.8;
	static constexpr float GREN_FUSE = 3;
	static constexpr float GREN_EXP_RADIUS = 70;
	static constexpr float GREN_EXP_DMG = 45;

	float fuse_tmr = 0;
};


