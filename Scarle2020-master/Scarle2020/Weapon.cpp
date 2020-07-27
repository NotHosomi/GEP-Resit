#include "pch.h"
#include "Weapon.h"
#include "GameData.h"

Weapon::Weapon(ID3D11Device* _GD) :
	ImageGO2D("weapon", _GD)
{
	m_rotation = angle;
}

void Weapon::Tick(GameData* _GD)
{
	// set aiming by accessing _GD->m_Teams->CurrentUnit
	if (!can_fire || !aiming)
	{
		if (charge > 0)
		{
			chargeWeapon(_GD);
		}
		return;
	}
	// Player must be aiming, and not have already fired this turn

	// TODO: Next/Prev controls
	if (_GD->m_Input.checkKey(InputManager::IN_UP))
	{
		angle -= WEP_ROT_SPEED * _GD->m_dt;
		// TODO: check if we're flipped
		if (angle < AIM_MIN_ANGLE)
			angle = AIM_MIN_ANGLE;
		else if (angle > AIM_MAX_ANGLE)
			angle = AIM_MAX_ANGLE;
		m_rotation = angle;
	}
	if (_GD->m_Input.checkKey(InputManager::IN_DOWN))
	{
		angle += WEP_ROT_SPEED * _GD->m_dt;
		// TODO: check if we're flipped
		if (angle < AIM_MIN_ANGLE)
			angle = AIM_MIN_ANGLE;
		else if (angle > AIM_MAX_ANGLE)
			angle = AIM_MAX_ANGLE;
		m_rotation = angle;
	}
	if (_GD->m_Input.checkKey(InputManager::IN_FIRE))
	{
		can_fire = false;
		if (current_weptype == WEP_DYNAMITE)
		{
			fire(_GD);
			return;
		}
		chargeWeapon(_GD);
	}
}

void Weapon::chargeWeapon(GameData* _GD)
{
	charge += _GD->m_dt;
	if (charge > WEP_MAX_CHAGE_TIME ||
		!_GD->m_Input.checkKey(InputManager::IN_FIRE))
	{
		fire(_GD);
	}
}

void Weapon::fire(GameData* _GD)
{
	// Projectile* new_projectile
	switch (current_weptype)
	{
		case WEP_ROCKET:
			break;
		case WEP_PISTOL:
			break;
		case WEP_GRENADE:
			break;
		case WEP_DYNAMITE:
			break;
	}
	// _GD->carrier->add Projectile
}

void Weapon::setColour(WepType wep_id)
{
}

/// Note for late
// Some kind of object creation/deletion message system embedded in GameData perhaps?
// A delete vector, and you add "this" to the vector
// A creation vector, you create the object, then give the pointer to the vector