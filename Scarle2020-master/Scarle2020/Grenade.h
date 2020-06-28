#pragma once
#include "Projectile.h"

class Grenade : public Projectile
{
public:
	Grenade(ID3D11Device* _GD, Vector2 _pos, Vector2 initial_vel, float _power) : Projectile{ _GD, _pos, initial_vel, "grenade", _power, 10 }
	{
		m_width = 22;
		m_height = 12;
		m_physicsComp->setBounce(true);
	}
	~Grenade() = default;

	void Tick(GameData* _GD) override;

protected:
	float explode_time = 7;
	float elapsed_time = 0;
};

