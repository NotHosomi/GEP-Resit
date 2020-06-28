#pragma once
#include "Grenade.h"

class Frag : public Grenade
{
public:
	Frag(ID3D11Device* _GD, Vector2 _pos, Vector2 initial_vel) : Grenade{ _GD, _pos, initial_vel, 20 }
	{}
	~Frag() = default;

	void Explode();
};

