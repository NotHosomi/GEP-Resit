#pragma once

#include <string>
#include <SimpleMath.h>
#include "UIStates.h"
#include "WeaponData.h"
using namespace DirectX::SimpleMath;
struct BasicObject
{
	std::string name = "";
	Vector2 pos = Vector2(0, 0);
	Vector2 speed = Vector2(0, 0);
	Vector2 dimentions = Vector2(0, 0);
	float scale = 1.0f;
	ButtonTypes buttonType;
	std::string uiText = "";
	int animationCount = 0;
	Teams team;
	int playerID = 0;
	Color color;
	Vector2 textScale;
	//components
	//SIMON:: You could use a bit-mask here.
	//Or a Map data structure, based on a componet ENUM.
	bool hasSpriteComp = false;
	bool hasCollisionComp = false;
	bool hasPhysicsComp = false;
	bool hasButtonComp = false;
	bool hasTextComp = false;
	bool hasAnimationComp = false;
	bool hasPlayerComp = false;

	//weapons
	bool hasWeaponComp = false;
	WeaponData::AttackTypes attackType;
	WeaponData::AimTypes aimType;
	bool useGravity = true;
	float weight = 4;
	bool isBouncie = false;
	float exlpodeCount = 0;
	int damageAmount = 0;
	float explosionScale = 0;
	int ammoCount = 0;
	bool needsCharging = false;
	int fireRate = 0;
	int spread = 0;
	std::string weaponSprite = "";
	int weaponWidth = 0;
	int weaponHeight = 0;
};