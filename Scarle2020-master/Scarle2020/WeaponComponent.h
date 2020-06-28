#pragma once
#include "WeaponData.h"
#include "SpriteComponent.h"

#include <list>
class WeaponComponent
{
public:
	WeaponComponent(WeaponData::AttackTypes _attack, WeaponData::AimTypes _aim);
	~WeaponComponent() = default;

	WeaponData::AttackTypes getAttackType() { return m_attackType; }
	WeaponData::AimTypes getAimType() { return m_aimType; }

	//if homing
	float getTargetX() { return m_target_x; }
	float getTargetY() { return m_target_y; }
	void setTarget(float x, float y) { m_target_x = x; m_target_y = y; };
	bool getTargetSet() { return m_targetSet; }
	void setTargetSet(bool _value) { m_targetSet = _value; }
	
	void setAimAngle(float angle) { m_aimAngle = angle; };
	float getAimAngle() { return m_aimAngle; };
	
	void setCharge(float charge) { m_charge = charge; };
	float getCharge() { return m_charge; }
	bool requiresCharging() { return m_chargedAttack; };
	void setRequiresCharge(bool _value) { m_chargedAttack = _value; }

	bool timerOver() { return elapsed_time >= explode_time; }
	void updateTimer(float _DT) { elapsed_time += _DT; }
	void setExplodeTime(float _value) { explode_time = _value; }

	int damage() { return m_damage; }
	void damage(int _damage) { m_damage = _damage; }

	float explosionScale() { return m_explosionScale; }
	void explosionScale(float _scale) { m_explosionScale = _scale; }

	int ammoCount() { return m_ammo; }
	void ammoCount(int _ammo) { m_ammo = _ammo; }

	int fireRate() { return m_fireRate; }
	void fireRate(int _rate) { m_fireRate = _rate; }

	bool fireNextRound(float _dt);

	SpriteComponent* spriteComp(){ return m_spriteComp; }
	void AddSpriteComp(std::string _directory, ID3D11Device* _GD);

	void setWidth(int _value) { m_width = _value; }
	int getWidth() { return m_width; }
	void setHeight(int _value) { m_height = _value; }
	int getHeight() { return m_height; }

	void setSpread(int _value) { m_spread = _value; }
	int getSpread() { return m_spread; }
private:
	WeaponData::AttackTypes m_attackType;
	WeaponData::AimTypes m_aimType;

	SpriteComponent* m_spriteComp = nullptr;
	int m_width = 0;
	int m_height = 0;

	float m_aimAngle = 0.0f;;
	float m_charge = 0.0f;
	bool m_chargedAttack = true;

	//if homing
	float m_target_x = 0.0f;
	float m_target_y = 0.0f;
	bool m_targetSet = false;

	float elapsed_time = 0.0f;
	float explode_time = 2.0f;

	float m_explosionScale = 1.0f;
	int m_damage = 0;
	int m_ammo = 0;
	int m_fireRate = 0;
	int m_spread = 0;
	float m_delaySoFar = 0.0f;
	float m_fireRateDelay = 0.1f;
}; 
