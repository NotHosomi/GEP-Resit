#include "pch.h"
#include "Rocket.h"

Rocket::Rocket(ID3D11Device* _GD, Vector2 position, Vector2 velocity) :
	Projectile(_GD, "projectile0", position, velocity, Vector2(ROCKET_DIMS, ROCKET_DIMS), ROCKET_WEIGHT, 0, true)
{
	exp_radius = ROCKET_EXP_RADIUS;
	exp_damage = ROCKET_EXP_DMG;
}
