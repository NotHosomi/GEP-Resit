#pragma once

#include "ImageGO2D.h"
#include "PhysicsComponent.h"

class Grenade : public ImageGO2D
{
public:
	Grenade(ID3D11Device* _GD, const Vector2& location, const Vector2& velocity);

	void Tick(GameData* _GD) override;

	// debugging
	PhysicsComponent* getPhysCmp() { return &PhysCmp; };

private:
	// golden ratio 1 : 1.62
	static constexpr float GREN_DIMS = 10;
	static constexpr float GREN_WEIGHT = 70;
	static constexpr float GREN_ELASTICITY = 0.8;
	static constexpr float GREN_FUSE = 3;

	float fuse_tmr = 0;
	PhysicsComponent PhysCmp;
};


