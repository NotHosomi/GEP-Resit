#pragma once
#include "ImageGO2D.h"
class Weapon :
	public ImageGO2D
{
public:
	Weapon(ID3D11Device* _GD);
	void Tick(GameData* _GD) override;

	enum WepType
	{
		WEP_ROCKET = 0,
		WEP_PISTOL,
		WEP_GRENADE,
		WEP_DYNAMITE
		// TODO: add more
	};

private:
	static constexpr float AIM_MIN_ANGLE = 10;
	static constexpr float AIM_MAX_ANGLE = 170;
	static constexpr float WEP_ROT_SPEED = 5;
	static constexpr float WEP_DYNAMITE_SPEED = 20;
	static constexpr float WEP_MAX_CHAGE_TIME = 1.5;

	void chargeWeapon(GameData* _GD);
	void fire(GameData* _GD);
	void setColour(WepType wep_id);

	bool aiming = true; // the player is standing still
	bool can_fire = true;

	WepType current_weptype = WEP_ROCKET;
	float charge = 0;
	float angle = 90;
};

