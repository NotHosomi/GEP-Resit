#pragma once
#include <d3d11_1.h>
#include "GameObject2D.h"
#include <iostream>

class Projectile : public GameObject2D
{
public:
	Projectile(ID3D11Device* _GD, Vector2 _pos, Vector2 initial_vel, std::string sprite, float _speed, float _weight);
	~Projectile() = default;

	void Tick(GameData* _GD) override;
	void Move(GameData* _GD);
	void Explode();
	void updateAngle();

protected:
	//Vector2 velocity = Vector2(0, 0);
	float explosion_size = 2;
	float weight = 1;

};

