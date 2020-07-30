#pragma once
#include "ImageGO2D.h"
#include "PhysicsComponent.h"
class Projectile : public ImageGO2D
{
public:
	Projectile(ID3D11Device* _GD, string texture, Vector2 position, Vector2 velocity,
		Vector2 _dimensions, float _weight, float _elasticity);

	void Tick(GameData* _GD) override;

protected:
	virtual void explode(GameData* _GD);
	void OOBCheck(GameData* _GD);

	PhysicsComponent PhysCmp;
	float exp_radius = 100;
	float exp_damage = 60;
};

