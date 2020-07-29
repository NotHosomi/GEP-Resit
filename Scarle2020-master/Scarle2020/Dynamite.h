#pragma once
#include "grenade.h"
class Dynamite :
	public Grenade
{
public:
	Dynamite(ID3D11Device* _GD, Vector2 position, Vector2 velocity);
private:
	static constexpr float DYNAMITE_WEIGHT = 100;
	static constexpr float DYNAMITE_EXP_RADIUS = 100;
	static constexpr float DYNAMITE_EXP_DMG = 65;
	static constexpr float DYNAMITE_FUSE = 5;
};

