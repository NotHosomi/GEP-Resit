#include "pch.h"
#include "FileManager.h"

#include <iostream>

//Change "WORMS" to be loaded from file so not hard coded
//SIMON:: GOOD!
void FileManager::readJSON(std::string _directoryLevel, std::string _directoryWeapon, ID3D11Device* _GD, ID3D11DeviceContext* _deviceContext)
{
	std::ifstream file(_directoryLevel);
	json inFile = json::parse(file); 
	readLevelData(inFile, _GD, _deviceContext);

	std::ifstream weaponFile(_directoryWeapon);
	json wFile = json::parse(weaponFile);
	readWeaponData(wFile, _GD, _deviceContext);
}

void FileManager::readLevelData(json inFile, ID3D11Device* _GD, ID3D11DeviceContext* _deviceContext)
{
	m_gameData.m_title = jsonToString(inFile["WORMS"]["game"]);
	m_gameData.m_infot = jsonToString(inFile["WORMS"]["info"]);
	m_gameData.m_maxNumPlayers = jsonToInt(inFile["WORMS"]["player_amount"]);

	int levelCount = 0;
	int levelAmount = jsonToInt(inFile["WORMS"]["level_count"]);
	list<BasicObject> objects;
	for (int j = 0; j < levelAmount; ++j)
	{
		LevelData levelData = LevelData();
		objects.clear();
		levelData.id = j + 1;
		int objCount = jsonToInt(inFile["WORMS"]["level" + to_string(j + 1)]["object_count"]);
		levelData.wormsCount = objCount;
		levelData.playerCount = jsonToInt(inFile["WORMS"]["level" + to_string(j + 1)]["team_count"]);
		for (int i = 0; i < objCount; ++i)
		{
			BasicObject obj;
			obj.name = jsonToString(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["file_name"]);
			obj.pos.x = jsonToInt(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["x_pos"]);
			obj.pos.y = jsonToInt(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["y_pos"]);
			obj.speed.x = jsonToFloat(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["x_speed"]);
			obj.speed.y = jsonToFloat(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["y_speed"]);
			obj.scale = jsonToFloat(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["scale"]);
			obj.hasSpriteComp = jsonToBool(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["sprite_comp"]);
			obj.hasAnimationComp = jsonToBool(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["animation_comp"]);
			obj.hasCollisionComp = jsonToBool(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["collision_comp"]);
			obj.hasPhysicsComp = jsonToBool(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["physics_comp"]);
			obj.hasButtonComp = jsonToBool(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["button_comp"]);
			obj.hasPlayerComp = jsonToBool(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["player_comp"]);
			obj.buttonType = jsonToButtonType(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["ui_type"]);
			obj.dimentions.x = jsonToInt(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["width"]);
			obj.dimentions.y = jsonToInt(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["height"]);
			obj.animationCount = jsonToInt(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["animation_count"]);
			obj.hasTextComp = jsonToBool(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["text_comp"]);
			obj.team = jsonToTeamType(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["team"]);
			obj.uiText = jsonToString(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["text"]);
			obj.color = jsonToColour(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["text_color"]);
			obj.textScale.x = obj.textScale.y = jsonToFloat(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["text_scale"]);
			obj.playerID = jsonToInt(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["team_id"]);
			if (jsonToBool(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["is_terrain"]))
			{
				levelData.terrain = new GameObject2D();
				levelData.terrain->init(obj, _GD, _deviceContext);
			}
			else if (jsonToBool(inFile["WORMS"]["level" + to_string(j + 1)]["object" + to_string(i + 1)]["is_background"]))
			{
				levelData.background = new GameObject2D();
				levelData.background->init(obj, _GD, _deviceContext);
			}
			else
			{
				objects.push_back(obj);
			}
		}
		for (auto& obj : objects)
		{
			GameObject2D* gameObject = new GameObject2D();
			gameObject->init(obj, _GD, _deviceContext);
			levelData.objects.push_back(gameObject);
		}
		m_levels.push_back(levelData);
	}
}

void FileManager::readWeaponData(json inFile, ID3D11Device* _GD, ID3D11DeviceContext* _deviceContext)
{
	int weaponAmount = jsonToInt(inFile["WEAPONS"]["count"]);
	list<BasicObject> objects;
	for (int i = 0; i < weaponAmount;  i++)
	{
	
		BasicObject obj;
		obj.name = jsonToString(inFile["WEAPONS"][to_string(i + 1)]["sprite_directory"]);
		obj.hasSpriteComp = jsonToBool(inFile["WEAPONS"][to_string(i + 1)]["sprite_comp"]);
		obj.hasPhysicsComp = jsonToBool(inFile["WEAPONS"][to_string(i + 1)]["physics_comp"]);
		obj.useGravity = jsonToBool(inFile["WEAPONS"][to_string(i + 1)]["use_gravity"]);
		obj.isBouncie = jsonToBool(inFile["WEAPONS"][to_string(i + 1)]["is_bouncie"]);
		obj.speed.x = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["x_speed"]);
		obj.speed.y = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["y_speed"]);
		obj.aimType = jsonToAimTypes(inFile["WEAPONS"][to_string(i + 1)]["aim_type"]);
		obj.attackType = jsonToAttackTypes(inFile["WEAPONS"][to_string(i + 1)]["name"]);
		obj.hasWeaponComp = jsonToBool(inFile["WEAPONS"][to_string(i + 1)]["weapon_comp"]);
		obj.exlpodeCount = jsonToFloat(inFile["WEAPONS"][to_string(i + 1)]["explode_time"]);
		obj.damageAmount = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["damage"]);
		obj.explosionScale = jsonToFloat(inFile["WEAPONS"][to_string(i + 1)]["explosion_scale"]);
		obj.ammoCount = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["ammo"]);
		obj.dimentions.x = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["width"]);
		obj.dimentions.x = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["height"]);
		obj.scale = jsonToFloat(inFile["WEAPONS"][to_string(i + 1)]["scale"]);
		obj.needsCharging = jsonToBool(inFile["WEAPONS"][to_string(i + 1)]["charge"]);
		obj.fireRate = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["repetition"]);
		obj.weaponSprite = jsonToString(inFile["WEAPONS"][to_string(i + 1)]["worm_sprite"]);
		obj.weaponWidth = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["weapon_width"]);
		obj.weaponHeight = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["weapon_height"]);
		obj.spread = jsonToInt(inFile["WEAPONS"][to_string(i + 1)]["weapon_height"]);
		objects.push_back(obj);
	}

	for (auto& obj : objects)
	{
		GameObject2D* gameObject = new GameObject2D();
		gameObject->init(obj, _GD, _deviceContext);
		m_gameData.weapons.push_back(gameObject);
	}
}

void FileManager::AmendString(std::string& OUT_string, std::string _remove)
{
	OUT_string.replace(OUT_string.find(_remove), sizeof(_remove) - 1, "");
}

int FileManager::jsonToInt(std::string value)
{
	return std::stoi(value);
}

float FileManager::jsonToFloat(std::string value)
{
	return std::stof(value);
}

std::string FileManager::jsonToString(std::string value)
{
	return value;
}

bool FileManager::jsonToBool(std::string _value)
{
	if (_value == "true")
	{
		return true;
	}
	else 
	{
		return false;
	}
}

ButtonTypes FileManager::jsonToButtonType(std::string _value)
{
	ButtonTypes type = ButtonTypes::empty;
	
	if (_value == "quitGame")
	{
		type = ButtonTypes::quitGame;
	}
	else if (_value == "quitLevel")
	{
		type = ButtonTypes::quitLevel;
	}
	else if (_value == "loadNextScene")
	{
		type = ButtonTypes::loadNextScene;
	}
	else if (_value == "pauseScene")
		type = ButtonTypes::pauseScene;
	else if (_value == "nextMap")
		type = ButtonTypes::nextMap;
	else if (_value == "loadChosenScene")
		type = ButtonTypes::loadSceneChoose;
	return type;
}

Teams FileManager::jsonToTeamType(std::string _value)
{
	Teams team;
	if (_value == "red")
		team = Teams::RED;
	else if (_value == "green")
		team = Teams::GREEN;
	else if (_value == "blue")
		team = Teams::BLUE;
	else if (_value == "purple")
		team = Teams::PURPLE;
	else
		team = Teams::EMPTY;
	return team;
}

WeaponData::AimTypes FileManager::jsonToAimTypes(std::string _value)
{
	WeaponData::AimTypes type = WeaponData::AimTypes::UNAIMED;

	if (_value == "aimed")
		type = WeaponData::AimTypes::AIMED;
	else if (_value == "targetted")
		type = WeaponData::AimTypes::TARGETTED;

	return type;

}

WeaponData::AttackTypes FileManager::jsonToAttackTypes(std::string _value)
{
	WeaponData::AttackTypes type = WeaponData::AttackTypes::W_BAZOOKA;
	
	if (_value == "bazooka")
		type = WeaponData::AttackTypes::W_BAZOOKA;
	else if (_value == "grenade")
		type = WeaponData::AttackTypes::W_GRENADE;
	else if (_value == "homing_missile")
		type = WeaponData::AttackTypes::W_HOMINGMISSILE;
	else if (_value == "hand_gun")
		type = WeaponData::AttackTypes::W_HANDGUN;
	else if (_value == "dynamite")
		type = WeaponData::AttackTypes::W_DYNAMITE;
	else if (_value == "dig")
		type = WeaponData::AttackTypes::W_DIG;
	return type;
}

Color FileManager::jsonToColour(std::string _value)
{
	if (_value == "red")
		return Colors::Red;
	else if (_value == "blue")
		return Colors::Blue;
	else if (_value == "green")
		return Colors::Green;
	else if (_value == "purple")
		return Colors::Purple;
	else
		return Colors::White;
}
