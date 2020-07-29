#pragma once
#include "ImageGO2D.h"
#include "TeamsManager.h"

class Weapon :
	public ImageGO2D
{
public:
	Weapon(ID3D11Device* _GD);
	void Tick(GameData* _GD) override;
	void Draw(DrawData2D* _DD) override;

	void updateWepListHudElement(TeamsManager* _TM);

	enum WepType
	{
		WEP_ROCKET = 0,
		WEP_PISTOL,
		WEP_GRENADE,
		WEP_DYNAMITE
		// TODO: add more
	};

private:
	static constexpr float AIM_MIN_ANGLE = 0.174;
	static constexpr float AIM_MAX_ANGLE = 2.967; // angles in rads :(
	static constexpr float WEP_ROT_SPEED = 5;
	static constexpr float WEP_DYNAMITE_SPEED = 20;
	static constexpr float WEP_MAX_CHARGE_TIME = 1.5;
	static constexpr float WEP_NUMWEPS = 4;
	static constexpr float HUD_LIST_DECAY_TIME = 3;
	static constexpr float WEP0_CHARGE_MULT = 400;

	void switchWep(GameData* _GD, bool forward);
	void chargeWeapon(GameData* _GD);
	void fire(GameData* _GD);
	void changeAngle(GameData* _GD, bool is_up);
	void pickColour();
	Vector2 generateAimVector();

	bool has_fired = false;
	bool projectile_released = false;
	Weapon::WepType current_weptype = WEP_ROCKET;
	float charge = 0;
	float angle = 0.5 * 3.14159265; // start at 90degrees. can't find D3DX_PI for some reason

	float list_display_timer = 0;
	TextGO2D hud_weaponlist = TextGO2D("--");
};