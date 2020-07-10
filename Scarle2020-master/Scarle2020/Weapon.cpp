#include "pch.h"
#include "Weapon.h"
#include "Projectile.h"
#include "Weapons.h"

#include <iostream>
#include <string>

void Weapon::fire(ID3D11Device* _GD, list<GameObject2D*> &GO2Dlist, Vector2 owner, GameObject2D* _projectile)
{
	WeaponComponent* wComp = _projectile->weaponComponent();
	SpriteComponent* sComp = _projectile->spriteComp();
	PhysicsComponent* pComp = _projectile->physicsComponent();
	int amount = wComp->fireRate();
	int spread = wComp->getSpread();
		GameObject2D* copy = copyObject(_projectile);
		if(sComp)
			copy->copySpriteComp(sComp);
		if(pComp)
			copy->copyPhysicsComp(pComp);
		if(wComp)
			copy->copyWeaponComp(wComp);

		if (wComp->getCharge() > 0)
			copy->physicsComponent()->setVelocity(generateVelocity(wComp->getCharge() * CHARGE_MODI, wComp->getAimAngle()));
		else
			copy->physicsComponent()->setVelocity(generateVelocity(UNCHARGED_MODI, wComp->getAimAngle()));
		// HOS-TODO: Find bullet's disappearing vel

		copy->SetPos(Vector2(owner.x, owner.y - 15));
		if(copy->spriteComp())
			copy->spriteComp()->setVisibility(true);

		GO2Dlist.push_back(new GameObject2D(*copy));
		m_fireRateSoFar++;
}

void Weapon::MaintainProjectiles(list<GameObject2D*>& objects, GameData *_GD)
{
	for (auto& obj : objects)
	{
		if (obj->weaponComponent() && obj->spriteComp() && obj->spriteComp()->getVisibility())
		{
			WeaponData::AttackTypes type = obj->weaponComponent()->getAttackType();
			switch (type)
			{
			case WeaponData::AttackTypes::W_GRENADE:
				if (obj->physicsComponent()->getBounce() && obj->weaponComponent()->timerOver())
				{
					obj->physicsComponent()->setBounce(false);
				}
				break;
			case WeaponData::AttackTypes::W_HOMINGMISSILE:
				if (obj->weaponComponent()->timerOver() && obj->physicsComponent()->getGravity())
				{
					obj->physicsComponent()->moveToTarget(_GD->m_dt, obj->GetPos(),
														  Vector2(obj->weaponComponent()->getTargetX(),
														  obj->weaponComponent()->getTargetY()));
					obj->physicsComponent()->setGravity(false);
					obj->physicsComponent()->rotate(0.0f, XM_PI, _GD->m_dt);
					obj->physicsComponent()->setIsRotating(false);
				}
				break;
			case WeaponData::AttackTypes::W_HANDGUN:
				obj->physicsComponent()->rotate(0.0f, XM_PI, _GD->m_dt);
				obj->physicsComponent()->setIsRotating(false);
				break;
			case WeaponData::AttackTypes::W_DYNAMITE:
				obj->physicsComponent()->setIsRotating(false);
				obj->physicsComponent()->setVelX(0);
				obj->physicsComponent()->setVelY(0);
				if (obj->physicsComponent()->getBounce() && obj->weaponComponent()->timerOver())
				{
					obj->physicsComponent()->setBounce(false);
				}
				break;

			case WeaponData::AttackTypes::W_DIG:
				obj->physicsComponent()->rotate(0.0f, XM_PI, _GD->m_dt);
				obj->physicsComponent()->setIsRotating(false);
			}

			obj->physicsComponent()->move(_GD->m_dt, obj->GetPos());
			obj->physicsComponent()->rotate(0.0f, XM_PI, _GD->m_dt);
		}
	}
}

void Weapon::incrementShotsFired()
{
	m_shotsFired++;
	m_fireRateSoFar = 0;
}

GameObject2D* Weapon::copyObject(GameObject2D* original)
{
	GameObject2D* projectileCopy = new GameObject2D(*original);
	projectileCopy->AddCollisionComponent();
	RECT rect;
	rect.left = projectileCopy->GetPos().x;
	rect.top = projectileCopy->GetPos().y;
	rect.right = projectileCopy->GetPos().x + projectileCopy->getWidth();
	rect.bottom = projectileCopy->GetPos().y + projectileCopy->getHeight();
	projectileCopy->collisionComponent()->Init(rect);

	return projectileCopy;
}

Vector2 Weapon::generateVelocity(float _charge, float _angle)
{
	float y = -cos(_angle);
	float x = sin(_angle);
	return Vector2(x, y) * _charge;
}
