#pragma once
#include <d3d11_1.h>
#include "Projectile.h"

class Missile : public Projectile
{
public:
	Missile(ID3D11Device* _GD, Vector2 _pos, Vector2 initial_vel, float _power) : Projectile{ _GD, _pos, initial_vel, "missile", _power, 2 }
	{
		m_width = 22;
		m_height = 12;
	}

	void Tick(GameData* _GD) override;

private:


};

