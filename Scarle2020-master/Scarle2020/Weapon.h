#pragma once

#include "GameObject2D.h"
#include "WeaponData.h"
#include <list>

using std::list;

class Weapon
{
public:
	Weapon() = default;
	void setAimAngle(float angle) { m_aimAngle = angle; };
	float getAimAngle() { return m_aimAngle; };
	void setCharge(float charge) { m_charge = charge ; };

	void fire(ID3D11Device* _GD, list<GameObject2D*> & GO2Dlist, Vector2 owner, GameObject2D* _projectile);

	void MaintainProjectiles(list<GameObject2D*>& objects, GameData* _GD);

	void incrementShotsFired();
	void resetShotsFired() { m_shotsFired = 0; }
	int getShotsFired() { return m_shotsFired; }
	int getFireRateSoFar() { return m_fireRateSoFar; }
private:

	float m_aimAngle = 0.0f;
	float m_charge = 0.0f;

	int m_shotsFired = 0;
	int m_fireRateSoFar = 0;
	GameObject2D* copyObject(GameObject2D* original);

	Vector2 generateVelocity(float _charge, float _angle);
};

