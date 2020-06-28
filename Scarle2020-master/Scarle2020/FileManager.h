#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <list>

#include "json.hpp"
#include "DataTypes.h"
#include "ObjectData.h"
#include "WeaponData.h"

using json = nlohmann::json;

class FileManager
{
public:
	FileManager() = default;
	~FileManager() = default;
	
	void readJSON(std::string _directoryLevel, std::string _directoryWeapon, ID3D11Device* _GD, ID3D11DeviceContext* _deviceContext);

	MyGameData getGameData() { return m_gameData; }
	std::list<LevelData> getLevels() { return m_levels; }
private:
	void readLevelData(json inFile, ID3D11Device* _GD, ID3D11DeviceContext* _deviceContext);
	void readWeaponData(json inFile, ID3D11Device* _GD, ID3D11DeviceContext* _deviceContext);
	
	void AmendString(std::string& OUT_string, std::string _remove);
	int jsonToInt(std::string value);
	float jsonToFloat(std::string value);
	std::string jsonToString(std::string value);
	bool jsonToBool(std::string _value);
	ButtonTypes jsonToButtonType(std::string _value);
	Teams jsonToTeamType(std::string _value);
	WeaponData::AimTypes jsonToAimTypes(std::string _value);
	WeaponData::AttackTypes jsonToAttackTypes(std::string _value);
	Color jsonToColour(std::string _value);

	MyGameData m_gameData;
	std::list<LevelData> m_levels;
};

