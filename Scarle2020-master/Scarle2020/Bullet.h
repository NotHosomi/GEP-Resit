#pragma once
#include "Projectile.h"
class Bullet :
	public Projectile
{
public:
	Bullet(ID3D11Device* _GD, Vector2 position, Vector2 velocity);
private:
	static constexpr float BULLET_DIMS = 12;
	static constexpr float BULLET_WEIGHT = 150;
	static constexpr float BULLET_EXP_RADIUS = 20;
	static constexpr float BULLET_EXP_DMG = 25;
};

