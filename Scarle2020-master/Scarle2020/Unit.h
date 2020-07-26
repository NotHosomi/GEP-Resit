#pragma once

#include "ImageGO2D.h"
#include "PhysicsComponent.h"

class Unit : public ImageGO2D
{
public:
	Unit(ID3D11Device* _GD, const Vector2& location);

	void Tick(GameData* _GD) override;

	// TeamsManager controls
	void setAwake(bool _awake);
	int getTeam();

	// debugging
	PhysicsComponent* getPhysCmp() { return &PhysCmp; };

private:
	// golden ratio 1 : 1.62
	static constexpr float UNIT_HEIGHT = 26;
	static constexpr float UNIT_WIDTH = 16;
	static constexpr float UNIT_WEIGHT = 60;
	static constexpr float UNIT_ELASTICITY = 0.1;
	static constexpr float MV_ACCELERATION = 12;
	static constexpr float MV_AIRACCELERATION = 1;
	static constexpr float MV_MAXAIRCONTROL = 10;
	static constexpr float MV_JUMPFORCE_V = 60;
	static constexpr float MV_JUMPFORCE_H = 40;

	void playerMove(GameData* _GD);

	PhysicsComponent PhysCmp;
	bool awake = true; // TODO: set false
	bool alive = true;

	float health = 100;
	int team_id = 0;
	bool facing_right = true;
};

