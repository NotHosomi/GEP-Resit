#include "pch.h"
#include "GameManager.h"

GameManager::GameManager(ID3D11Device1* _device)
{
	if (m_uiManager)
	{
		delete(m_uiManager);
	}
	m_uiManager = new UIManager();

	if (m_teamManager)
	{
		delete(m_teamManager);
	}
	m_teamManager = new TeamManager();
	//play
	//SIMON:: Hard coded buttons, could be a bit more data driven, by having a UI manager which owns these like out GameObject"D data structure.
	//ALthough looks like one is on its way.
	
	m_playButton = new GameObject2D();
	m_playButton->SetSprite("button", _device);
	m_playButton->AddButtonComponent(ButtonTypes::play);
	m_playButton->AddTextComponent();
	m_playButton->textComponent()->text("CONTINUE");
	m_playButton->textComponent()->setColour(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	m_playButton->spriteComp()->setVisibility(false);
	m_playButton->SetPos(Vector2(700, 300));
	m_playButton->SetScale(0.7f);
	//SIMON:: Effectively hard coded positions/sizes.
	m_playButton->setWidth(640 * m_playButton->GetScale().x);
	m_playButton->setheight(320 * m_playButton->GetScale().y);
	m_playButton->buttonComponent()->addTextComponent();
	m_playButton->buttonComponent()->textComponent()->text("CONTINUE");
	m_playButton->buttonComponent()->textComponent()->setColour(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	m_playButton->textComponent()->setScale(Vector2(2.0f, 2.0f));
	m_playButton->textComponent()->setXOffSet(-90);
	m_playButton->AddCollisionComponent();
	RECT rect;
	rect.left = 300;
	rect.right = 300 + m_playButton->getWidth();
	rect.top = 300;
	rect.bottom = 300 + m_playButton->getHeight();
	m_playButton->collisionComponent()->Init(rect);
	m_playButton->collisionComponent()->updateRect(300, 300, 640, 320);


	m_exitButton = new GameObject2D();
	m_exitButton->SetSprite("button", _device);
	m_exitButton->AddButtonComponent(ButtonTypes::loadMainMenu);
	m_exitButton->AddTextComponent();
	m_exitButton->textComponent()->text("EXIT");
	m_exitButton->textComponent()->setColour(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	m_exitButton->spriteComp()->setVisibility(false);
	m_exitButton->SetPos(Vector2(700, 600));
	m_exitButton->SetScale(0.7f);
	m_exitButton->setWidth(640 * m_exitButton->GetScale().x);
	m_exitButton->setheight(320 * m_exitButton->GetScale().y);
	m_exitButton->buttonComponent()->addTextComponent();
	m_exitButton->buttonComponent()->textComponent()->text("EXIT");
	m_exitButton->buttonComponent()->textComponent()->setColour(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	m_exitButton->textComponent()->setScale(Vector2(2.0f, 2.0f));
	m_exitButton->textComponent()->setXOffSet(40);
	m_exitButton->AddCollisionComponent();
	RECT rectA;

	rectA.left = 300;
	rectA.right = 300 + m_exitButton->getWidth();
	rectA.top = 300;
	rectA.bottom = 300 + m_exitButton->getHeight();
	m_exitButton->collisionComponent()->Init(rectA);
	m_exitButton->collisionComponent()->updateRect(300, 300, 640, 320);



}

void GameManager::initGame(MyGameData _gameData, std::list<LevelData> _allLevels)
{
	m_gameData = _gameData;
	for (auto& levels : _allLevels)
	{
		m_allLevels.push_back(levels);
	}
}

void GameManager::loadStartLevel()
{
	m_currentLevel = &m_allLevels.front();
	m_uiManager->adUI(m_playButton);
	m_uiManager->updateSceneData(m_currentLevel->objects);
	//testing
	//m_currentWorm = m_currentLevel->objects.front();
}

void GameManager::loadNextLevel()
{
	bool found = false;
	for (auto& level : m_allLevels)
	{
		if (found)
		{
			m_currentLevel = &level;
			m_uiManager->updateSceneData(m_currentLevel->objects);
			m_uiManager->adUI(m_playButton);
			m_uiManager->adUI(m_exitButton);
			m_currentLevel->objects.push_back(m_playButton);
			m_currentLevel->objects.push_back(m_exitButton);
			break;
		}
		if (level.id == m_currentLevel->id)
		{
			found = true;
		}
	}
	//nextWorm();
	m_teamManager->setTeamCount(m_currentLevel->playerCount);
	nextPlayer();
}

void GameManager::loadLevel(int _id)
{
	for (auto &level : m_allLevels)
	{
		if (level.id == _id)
		{
			m_currentLevel = &level;
			m_uiManager->updateSceneData(m_currentLevel->objects);
			m_uiManager->adUI(m_playButton);
			m_currentLevel->objects.push_back(m_playButton);
			m_uiManager->adUI(m_exitButton);
			m_currentLevel->objects.push_back(m_exitButton);
			break;
		}
	}
	//nextWorm();
	m_teamManager->setTeamCount(m_currentLevel->playerCount);
	nextPlayer();
}

void GameManager::pauseScene()
{
	//set menu buttons visible and active
	m_playButton->spriteComp()->setVisibility(true);
	m_exitButton->spriteComp()->setVisibility(true);
	/*for(auto &button : m_currentLevel->objects)
	{
		if (button->buttonComponent() && button->buttonComponent()->getButtonType  &ButtonTypes::pauseScene)
		{
			button->spriteComp->setVisibility(false);
		}
	}*/
}


void GameManager::nextWorm()
{
	if (m_currentLevel->id < 3)
		return;
	Teams team = Teams::BLUE;
	if (m_currentWorm)
		team = m_currentWorm->playerComponent()->getTeam();
	int teamNumber = (int)team;
	Teams nextTeam;
	bool found = false;
	while (!found)
	{
		if (teamNumber != 3)
			nextTeam = Teams(++teamNumber);
		else
			nextTeam = Teams(0);

		for (auto& obj : m_currentLevel->objects)
		{
			if (obj->playerComponent() && obj->playerComponent()->getTeam() == nextTeam)
			{
				m_currentWorm = obj;
				found = true;
				break;
			}
		}
	}
}

void GameManager::nextPlayer()
{
	if (m_currentLevel->id < 3 || m_currentLevel->id == m_allLevels.back().id)
		return;

	bool wormFound = false;
	Teams team = Teams::RED;
	Teams nextTeam;
	while (!wormFound)
	{
		//find next team
		if (m_currentWorm)
			team = m_currentWorm->playerComponent()->getTeam();
		int teamNumber = (int)team;
		m_teamManager->updateTurn(team);
		if (teamNumber != 3)
			nextTeam = Teams(++teamNumber);
		else
			nextTeam = Teams(0);

		int nextWormID = 0;
		switch (nextTeam)
		{
		case Teams::RED:
			nextWormID = m_teamManager->getNextRed();
			break;
		case Teams::GREEN:
			nextWormID = m_teamManager->getNextGreen();
			break;
		case Teams::BLUE:
			nextWormID = m_teamManager->getNextBlue();
			break;
		case Teams::PURPLE:
			nextWormID = m_teamManager->getNextPurple();
			break;
		}


		for (auto& player : m_currentLevel->objects)
		{
			if (player->playerComponent() && player->playerComponent()->getTeamStruct()->id == nextWormID
				&& player->playerComponent()->getTeamStruct()->colour == nextTeam)
			{
				m_currentWorm = player;
				break;
			}
		}
		if (m_currentWorm && m_currentWorm->playerComponent()->health() > 0)
			wormFound = true;
	}	

}
	

void GameManager::nextWeapon()
{
	if (!m_currentWeapon || m_currentWeapon == m_gameData.weapons.back())
	{
		m_currentWeapon = m_gameData.weapons.front();
		return;
	}
	
	bool found = false;
	for (auto& weapon : m_gameData.weapons)
	{
		if (found)
		{
			m_currentWeapon = weapon;
			return;
		}
		if (weapon == m_currentWeapon)
		{
			found = true;
		}
	}
}

void GameManager::previousWeapon()
{
	if (m_currentWeapon == m_gameData.weapons.front())
	{
		m_currentWeapon = m_gameData.weapons.back();
		return;
	}

	bool found = false;
	for (std::list<GameObject2D*>::reverse_iterator it = m_gameData.weapons.rbegin();
		it != m_gameData.weapons.rend(); ++it) 
	{
		if (found)
		{
			m_currentWeapon = (*it);
			return;
		}
		if ((*it) == m_currentWeapon)
		{
			found = true;
		}
	}
}

void GameManager::updateScene(Vector2 _mousePos, GameData* _GD, ID3D11Device1* _device, list<GameObject2D*>& _alphaRemove)
{
	//if clicked only
	m_currentClicked = m_uiManager->checkUIClicked(_mousePos);
	if (m_currentClicked != ButtonTypes::empty)
	{
		executeButtons(_GD, _device, _alphaRemove);
	}
}

void GameManager::executeButtons(GameData* _GD, ID3D11Device1* _device, list<GameObject2D*>& _alphaRemove)
{
	switch (m_currentClicked)
	{
	case ButtonTypes::quitGame:
		//quit game
		PostQuitMessage(0);
		break;
	case ButtonTypes::quitLevel:
		//back to start menu
		loadStartLevel();
		break;
	case ButtonTypes::loadNextScene:
		//load next level
		loadNextLevel();
		if(m_currentLevel->id > 2 || m_currentLevel->id == lastLevel()->id)
			_GD->m_GS = GS_PLAY_MAIN_CAM;
		break;
	case ButtonTypes::pauseScene:
		//pause game events
		pauseScene();
		_GD->m_GS = GS_PAUSE;
		m_currentLevel->objects;
		break;
	case ButtonTypes::addWorm:
		//add worm to perticular team
		break;
	case ButtonTypes::teleportOn:
		//add bool for teleport 
		break;
	case ButtonTypes::turnTime:
			//add Turn Time switch
		break;
	case ButtonTypes::startHealth:
		//add function which dictates starting health
		break;
	case ButtonTypes::play:
		_GD->m_GS = GS_PLAY_MAIN_CAM;
		m_playButton->spriteComp()->setVisibility(false);
		m_exitButton->spriteComp()->setVisibility(false);
		//de activate buttons
		break;
	case ButtonTypes::nextMap:
		//update map image
		//queue up level id
		updateLevelHolder(_device);
		break;
	case ButtonTypes::loadSceneChoose:
		loadLevel(m_levelID);
		if (m_currentLevel->id > 2 && m_currentLevel->id < lastLevel()->id)
			_GD->m_GS = GS_PLAY_MAIN_CAM;
		break;
	case ButtonTypes::loadMainMenu:
		loadLevel(1);
		m_exitButton->spriteComp()->setVisibility(false);
		m_playButton->spriteComp()->setVisibility(false);
		_alphaRemove.clear();
		_GD->m_GS = GS_MAIN_MENU;
	}
}

void GameManager::updateLevelHolder(ID3D11Device1* _device)
{
	std::string terrain = "";
	if (m_levelID == m_allLevels.back().id - 1)
	{
		m_levelID = 3;
	}
	else
	{
		m_levelID++;
	}
	for (auto& level : m_allLevels)
	{

		if (level.id == m_levelID)
		{
			terrain = level.terrain->spriteComp()->getDirectory();
			break;
		}
	}
	for (auto &level : m_allLevels)
	{

		if (level.id == 2)
		{
			for (auto& obj : level.objects)
			{
				if (obj->spriteComp() && !obj->buttonComponent() && !obj->textComponent() && !obj->playerComponent() && obj->GetPos().x > 0)
				{
					obj->SetSprite(terrain, _device);
					break;
				}
			}
		}
	}
}
