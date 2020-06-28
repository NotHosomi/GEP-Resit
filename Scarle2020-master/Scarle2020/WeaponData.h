#pragma once
struct WeaponData
{
	enum AimTypes {
		AIMED = 0,
		UNAIMED,
		TARGETTED,
		U_HOMINGMISSILE,// unique aiming for homing missile - targetted AND aimed
		U_KAMIKAZE			// unique aiming for kamikaze - locked to specific angles
	};
	enum AttackTypes {
		W_BAZOOKA = 0,
		W_GRENADE,
		W_CLUSTERGRENADE,
		W_MINE,
		W_HOMINGMISSILE,
		W_DYNAMITE,
		W_HANDGUN,
		W_DIG
	};
	WeaponData::AimTypes m_aimType;

	//charging
	bool m_chargedAttack;

	// The attack to occur after fire is released
	WeaponData::AttackTypes m_AttackType;
};