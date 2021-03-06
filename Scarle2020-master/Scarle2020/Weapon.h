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
		WEP_DYNAMITE,
		WEP_CLUSTER,
		WEP_AIRSTRIKE
		// TODO: add more
	};

private:
	static constexpr float AIM_MIN_ANGLE = 0.174;
	static constexpr float AIM_MAX_ANGLE = 2.967; // angles in rads :(
	static constexpr float WEP_ROT_SPEED = 1;
	static constexpr float WEP_DYNAMITE_SPEED = 20;
	static constexpr float WEP_MAX_CHARGE_TIME = 1.5;
	static constexpr int WEP_NUMWEPS = 6;
	static constexpr float HUD_LIST_DECAY_TIME = 3;
	// using separate values as arrays cant be static consts
	static constexpr float WEP0_SPEED = 400; // rocket
	static constexpr float WEP1_SPEED = 600; // pistol
	static constexpr float WEP2_SPEED = 300; // grenade
	static constexpr float WEP3_SPEED = 10; // dynamite
	static constexpr float WEP4_SPEED = 300; // cluster
	static constexpr float WEP5_SPEED = 300; // airstrike
	static constexpr float PI = 3.14159265; // can't find D3DX_PI for some reason

	string getWeaponName(int wep_id);
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
	float angle = 0.5 * PI; // start at 90degrees.

	float list_display_timer = 0;
	TextGO2D hud_weaponlist = TextGO2D("--");

	bool draw_aimer = false;
	ImageGO2D aim_indicator;
	ImageGO2D charge_indicator;
};