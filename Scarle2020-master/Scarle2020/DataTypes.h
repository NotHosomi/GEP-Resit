#pragma once
#include <list>

#include "GameObject2D.h"
#include "UIStates.h"

struct MyGameData
{
	std::string m_title = "";
	std::string m_infot = "";
	int m_maxNumPlayers = 0;
	std::list<GameObject2D*> weapons;
};

struct LevelData
{
	int id = -1;
	int wormsCount = 0;
	int playerCount = 0;
	std::list<GameObject2D*> objects;
	GameObject2D* terrain = nullptr; 
	GameObject2D* background = nullptr;
};


