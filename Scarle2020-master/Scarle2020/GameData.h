#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

//=================================================================
//Data to be passed by game to all Game Objects via Tick
//=================================================================

#include "Keyboard.h"
#include "Mouse.h"
#include "Grid.h"
#include "GameState.h"
#include "InputManager.h"
#include "TeamsManager.h"

using namespace DirectX;

struct GameData
{
	float m_dt;  //time step since last frame
	GameState m_GS = GS_PLAY_MAIN_CAM;

	//player input
	Keyboard::State m_KBS;
	Mouse::State m_MS; 
	Keyboard::KeyboardStateTracker m_KBS_tracker;

	ID3D11Device* p_Device = nullptr;
	Grid* p_World = nullptr;
	InputManager m_Input;
	TeamsManager m_Teams;
	vector<GameObject2D*> creation_list;
	vector<GameObject2D*> deletion_list;
};
#endif
