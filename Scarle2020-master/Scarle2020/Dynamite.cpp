#include "pch.h"
#include "Dynamite.h"

Dynamite::Dynamite(ID3D11Device* _GD, Vector2 position, Vector2 velocity) :
	Grenade(_GD, position, velocity, DYNAMITE_WEIGHT, 0)
{
	exp_radius = DYNAMITE_EXP_RADIUS;
	exp_damage = DYNAMITE_EXP_DMG;
	fuse = DYNAMITE_FUSE;
}
