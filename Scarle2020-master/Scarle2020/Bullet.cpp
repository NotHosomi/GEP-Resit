#include "pch.h"
#include "Bullet.h"

Bullet::Bullet(ID3D11Device* _GD, Vector2 position, Vector2 velocity, float dmg, float radius) :
	Projectile(_GD, "projectile1", position, velocity, Vector2(BULLET_DIMS, BULLET_DIMS), 0, 0)
{
	exp_radius = radius;
	exp_damage = dmg;
}
