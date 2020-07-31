#include "pch.h"
#include "Weapon.h"
#include "GameData.h"
#include "DrawData2D.h"
#include "TeamsManager.h"
#include "Rocket.h"
#include "Bullet.h"
#include "grenade.h"
#include "Dynamite.h"
#include "Cluster.h"

Weapon::Weapon(ID3D11Device* _GD) :
	ImageGO2D("weapon", _GD),
	aim_indicator("aim_indicator", _GD),
	charge_indicator("charge_indicator", _GD)
{
	m_rotation = angle;
	aim_indicator.SetRot(angle);
	charge_indicator.SetRot(angle);
	pickColour();
	hud_weaponlist.SetColour(Color((float*)&Colors::OrangeRed));
	hud_weaponlist.SetScale(0.5);
	hud_weaponlist.SetPos(Vector2(0, 50));
}

void Weapon::Tick(GameData* _GD)
{
	m_pos = _GD->m_Teams.getCurrentUnit()->GetPos();
	aim_indicator.SetPos(m_pos);
	charge_indicator.SetPos(m_pos);


	if (_GD->m_Turn.getState() == TurnManager::TurnState::TS_PRE)
	{
		// Only needs to happen once, but this is good enough for now
		if (has_fired)
		{
			has_fired = false;
			projectile_released = false;
			charge = 0;
		}
		angle = 0.5 * PI;
		// update the weapon and sprite colour
		current_weptype = WEP_ROCKET;
		pickColour();
		// Wep List display timer
		updateWepListHudElement(&_GD->m_Teams);
		list_display_timer = HUD_LIST_DECAY_TIME;
	}

	// Flip the angle if the player has just flipped
	if (_GD->m_Teams.getCurrentUnit()->isFlipped())
	{
		if (angle > 0)
		{
			angle *= -1;
			m_rotation = angle;
			aim_indicator.SetRot(angle);
			charge_indicator.SetRot(angle);
		}
	}
	else
	{
		if (angle < 0)
		{
			angle *= -1;
			m_rotation = angle;
			aim_indicator.SetRot(angle);
			charge_indicator.SetRot(angle);
		}
	}

	if (list_display_timer)
	{
		list_display_timer -= _GD->m_dt;
		if (list_display_timer < 0)
		{
			list_display_timer = 0;
		}
	}

	// We've already fired. Just sleep
	if (projectile_released)
	{
		return;
	}
	// Fire is held, charging up
	if (charge > 0)
	{
		chargeWeapon(_GD);
		return;
	}
	// Player must be stood still
	if (!_GD->m_Teams.getCurrentUnit()->getPhysCmp()->isStill())
	{
		// hide the aim helper
		draw_aimer = false;
		return;
	}
	// Enable the aim helper if we're in the act phase 
	draw_aimer = (_GD->m_Turn.getState() == TurnManager::TurnState::TS_ACT);

	if (_GD->m_Input.checkKey(InputManager::IMP_NEXT))
	{
		switchWep(_GD, true);
	}
	if (_GD->m_Input.checkKey(InputManager::IMP_PREV))
	{
		switchWep(_GD, false);
	}
	if (_GD->m_Input.checkKey(InputManager::IN_UP))
	{
		changeAngle(_GD, true);
	};
	if (_GD->m_Input.checkKey(InputManager::IN_DOWN))
	{
		changeAngle(_GD, false);
	}
	if (_GD->m_Input.checkKey(InputManager::IN_FIRE))
	{
		list_display_timer = HUD_LIST_DECAY_TIME + WEP_MAX_CHARGE_TIME;
		has_fired = true;
		_GD->m_Teams.getCurrentUnit()->getPhysCmp()->setLocked(true);
		if (current_weptype == WEP_PISTOL || current_weptype == WEP_DYNAMITE)
		{
			fire(_GD);
			_GD->m_Input.releaseKey(InputManager::IN_FIRE);
			return;
		}
		list_display_timer = HUD_LIST_DECAY_TIME + WEP_MAX_CHARGE_TIME;
		chargeWeapon(_GD);
	}
}

void Weapon::Draw(DrawData2D* _DD)
{
	if (!has_fired)
	{
		_DD->m_Sprites->Draw(m_pTextureRV, m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
	}
	if (list_display_timer)
	{
		hud_weaponlist.Draw(_DD);
	}
	if (draw_aimer)
	{
		aim_indicator.Draw(_DD);
	}
	if (charge)
	{
		charge_indicator.Draw(_DD);
	}
}

void Weapon::updateWepListHudElement(TeamsManager* _TM)
{
	// This could be optimized with a lot of small functions
	// But it seems like overkill given the time constraint
	int amount;
	string str = "";
	if (current_weptype == 0)
		str += ">";
	str += "BAZOOKA: ";
	amount = _TM->ammoCount(0);
	str += amount < 0 ? "INFINITE" : to_string(amount);
	str += "\n";

	if (current_weptype == 1)
		str += ">";
	str += "PISTOL: ";
	amount = _TM->ammoCount(1);
	str += amount < 0 ? "INFINITE" : to_string(amount);
	str += "\n";

	if (current_weptype == 2)
		str += ">";
	str += "GRENADE: ";
	amount = _TM->ammoCount(2);
	str += amount < 0 ? "INFINITE" : to_string(amount);
	str += "\n";

	if (current_weptype == 3)
		str += ">";
	str += "DYNAMITE: ";
	amount = _TM->ammoCount(3);
	str += amount < 0 ? "INFINITE" : to_string(amount);

	hud_weaponlist.SetString(str);
}

void Weapon::switchWep(GameData* _GD, bool forward)
{
	int wep_id = static_cast<int>(current_weptype);
	do
	{
		wep_id += forward ? 1 : -1;
		// secure bounds
		if (wep_id >= WEP_NUMWEPS)
		{
			wep_id = 0;
		}
		else if (wep_id < 0)
		{
			wep_id = WEP_NUMWEPS - 1;
		}
		// check if the current weapon has ammo
		// -1 (infinite ammo) is cast to true
	} while (!_GD->m_Teams.ammoCount(wep_id));
	// update the weapon and sprite colour
	current_weptype = static_cast<Weapon::WepType>(wep_id);
	pickColour();
	// Wep List display timer
	updateWepListHudElement(&_GD->m_Teams);
	list_display_timer = HUD_LIST_DECAY_TIME;
}

void Weapon::chargeWeapon(GameData* _GD)
{
	charge += _GD->m_dt;
	charge_indicator.SetScale(charge / WEP_MAX_CHARGE_TIME);
	if (charge > WEP_MAX_CHARGE_TIME ||
		!_GD->m_Input.checkKey(InputManager::IN_FIRE))
	{
		fire(_GD);
	}
}

void Weapon::fire(GameData* _GD)
{
	list_display_timer = HUD_LIST_DECAY_TIME;
	_GD->m_Teams.consumeAmmo(current_weptype);
	updateWepListHudElement(&_GD->m_Teams);
	_GD->m_Teams.getCurrentUnit()->getPhysCmp()->setLocked(false);
	draw_aimer = false;

	GameObject2D* new_projectile = nullptr;
	switch (current_weptype)
	{
	case WEP_ROCKET: new_projectile =
		new Rocket(_GD->p_Device, _GD->m_Teams.getCurrentUnit()->GetPos(), generateAimVector() * WEP0_SPEED);
		break;
	case WEP_PISTOL: new_projectile =
		new Bullet(_GD->p_Device, _GD->m_Teams.getCurrentUnit()->GetPos(), generateAimVector() * WEP1_SPEED);
		break;
	case WEP_GRENADE: new_projectile =
		new Grenade(_GD->p_Device, _GD->m_Teams.getCurrentUnit()->GetPos(), generateAimVector() * WEP2_SPEED);
		break;
	case WEP_DYNAMITE:
	{
		float dir = _GD->m_Teams.getCurrentUnit()->isFlipped() ? -1 : 1;
		new_projectile = new Grenade(_GD->p_Device, _GD->m_Teams.getCurrentUnit()->GetPos(), Vector2(WEP3_SPEED * dir, 0));
	}
		break;
	case WEP_CLUSTER: new_projectile =
		new Cluster(_GD->p_Device, _GD->m_Teams.getCurrentUnit()->GetPos(), generateAimVector() * WEP4_SPEED);
		break;
	//case WEP_AIRSTRIKE: new_projectile =
	//	new Grenade(_GD->p_Device, _GD->m_Teams.getCurrentUnit()->GetPos(), generateAimVector() * WEP5_SPEED);
	}
	_GD->creation_list.emplace_back(new_projectile);
	charge = 0;
	projectile_released = true;

	// end the turn
	_GD->m_Turn.nextStage(&_GD->m_Teams);
	_GD->m_Turn.setWaiting(true);	
}

void Weapon::changeAngle(GameData* _GD, bool is_up)
{
	bool flipped = _GD->m_Teams.getCurrentUnit()->isFlipped();
	int modi = 1;
	// bool XOR
	if (is_up != flipped)
	{
		modi = -1;
		// we only invert the increment if doing down when
		// facing right, or when going up when facing left
	}
	angle += WEP_ROT_SPEED * _GD->m_dt * modi;

	if (flipped)
	{
		if (angle > 0)
		{
			angle *= -1;
		}
		if (angle > -AIM_MIN_ANGLE)
			angle = -AIM_MIN_ANGLE;
		else if (angle < -AIM_MAX_ANGLE)
			angle = -AIM_MAX_ANGLE;
	}
	else
	{
		if (angle < 0)
		{
			angle *= -1;
		}
		if (angle < AIM_MIN_ANGLE)
			angle = AIM_MIN_ANGLE;
		else if (angle > AIM_MAX_ANGLE)
			angle = AIM_MAX_ANGLE;
	}
	m_rotation = angle;
	aim_indicator.SetRot(angle);
	charge_indicator.SetRot(angle);
}

void Weapon::pickColour()
{
	switch (current_weptype)
	{
	case WEP_ROCKET:
		m_colour = Color((float*)&Colors::DarkOliveGreen);
		break;
	case WEP_PISTOL:
		m_colour = Color((float*)&Colors::Yellow);
		break;
	case WEP_GRENADE:
		m_colour = Color((float*)&Colors::Orange);
		break;
	case WEP_DYNAMITE:
		m_colour = Color((float*)&Colors::Maroon);
		break;
	}
}

Vector2 Weapon::generateAimVector()
{
	float x = sin(angle);
	float y = -cos(angle);
	float boost = charge > 0 ? charge : WEP_MAX_CHARGE_TIME;
	return Vector2(x, y) * boost;
}


// TODO: charge indicator