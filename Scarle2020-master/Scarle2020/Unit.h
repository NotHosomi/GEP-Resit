#pragma once

#include "ImageGO2D.h"
#include "PhysicsComponent.h"

class Unit : public ImageGO2D
{
public:
	Unit(ID3D11Device* _GD, const Vector2& location);

	void Tick(GameData* _GD) override;

	// debugging
	PhysicsComponent* getPhysCmp() { return &PhysCmp; };

private:
	// golden ratio 1 : 1.62
	static constexpr float UNIT_HEIGHT = 26;
	static constexpr float UNIT_WIDTH = 16;
	static constexpr float UNIT_WEIGHT = 8;

	PhysicsComponent PhysCmp;
};

