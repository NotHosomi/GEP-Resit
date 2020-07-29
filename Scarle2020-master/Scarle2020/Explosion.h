#pragma once
#include "ImageGO2D.h"
class Explosion :
	public ImageGO2D
{
public:
	Explosion(ID3D11Device* _GD, Vector2 position, float _radius, float _damage);
	void Tick(GameData* _GD) override;

private:
	static constexpr float EXP_KNOCKBACK_MULTIPLIER = 10;
	float radius = 1;
	float damage = 1;
};

