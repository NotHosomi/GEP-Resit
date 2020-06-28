#include "pch.h"
#include "WeaponComponent.h"

WeaponComponent::WeaponComponent(WeaponData::AttackTypes _attack, WeaponData::AimTypes _aim)
{
	m_attackType = _attack;
	m_aimType = _aim;
}

bool WeaponComponent::fireNextRound(float _dt)
{
	m_delaySoFar += _dt;
	if (m_delaySoFar > m_fireRateDelay)
	{
		m_delaySoFar = 0;
		return true;
	}
	return false;
}

void WeaponComponent::AddSpriteComp(std::string _directory, ID3D11Device* _GD)
{
	if(m_spriteComp)
		delete(m_spriteComp);
	m_spriteComp = new SpriteComponent(_directory,_GD);
}

