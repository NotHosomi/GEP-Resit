#pragma once
#include "Missile.h"

class HomingMissile : public Missile
{
public:
	HomingMissile(ID3D11Device* _GD, Vector2 _pos, Vector2 initial_vel, float _power, Vector2 _target);
	~HomingMissile() = default;

	void Tick(GameData* _GD) override;

protected:
	float elapsed_time = 0;
	float homing_time = 1;
	float delay_time = 5.2f;

	bool homing = false;

	Vector2 target_pos;
};

