#pragma once
#include "ImageGO2D.h"
#include "PhysicsComponent.h"
class Projectile : public ImageGO2D
{
public:
	Projectile(ID3D11Device* _GD, string texture, Vector2 position, Vector2 velocity,
		Vector2 _dimensions, float _weight, float _elasticity, bool _explode_on_contact);

	void Tick(GameData* _GD) override;

protected:
	virtual void explode(GameData* _GD);
	PhysicsComponent PhysCmp;
	bool explode_on_contact = false;
	float exp_radius = 100;
	float exp_damage = 60;
};

