#pragma once
#include <string>
#include <list>

#include "DataTypes.h"
#include "UIManager.h"
#include "TeamManager.h"

class GameManager
{
public:
	GameManager(ID3D11Device1* _device);
	~GameManager() = default;
	
	void initGame(MyGameData _gameData, std::list<LevelData> _allLevels);
	void loadStartLevel();
	void loadNextLevel();
	void loadLevel(int _id);
	void pauseScene();

	void currentWorm(GameObject2D* _worm) { m_currentWorm = _worm; }	
	GameObject2D* currentWorm() { return m_currentWorm; }
	void nextWorm();
	void nextPlayer();

	GameObject2D* currentWeapon() { return m_currentWeapon; }
	void nextWeapon();
	void previousWeapon();

	LevelData* currentLevel() { return m_currentLevel; }
	LevelData* lastLevel() { return &m_allLevels.back(); }
	MyGameData* gameData() { return &m_gameData; }

	void updateScene(Vector2 _mousePos, GameData* _GD, ID3D11Device1* _device, list<GameObject2D*>& _alphaRemove);
	void executeButtons(GameData* _GD, ID3D11Device1* _device, list<GameObject2D*>& _alphaRemove);

	TeamManager* teamManager() { return m_teamManager; };
private:
	void updateLevelHolder(ID3D11Device1* _device);

	MyGameData m_gameData;
	LevelData* m_currentLevel = nullptr;
	std::list<LevelData> m_allLevels;

	GameObject2D* m_currentWeapon = nullptr;
	GameObject2D* m_currentWorm = nullptr;
	ButtonTypes m_currentClicked;
	
	int m_levelID = 3;
	//managers
	UIManager* m_uiManager = nullptr;
	TeamManager* m_teamManager = nullptr;
	//pause buttons
	GameObject2D* m_playButton = nullptr;
	GameObject2D* m_exitButton = nullptr;
};

