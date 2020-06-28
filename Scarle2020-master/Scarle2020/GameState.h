#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

//=================================================================
//Possible GameStates
//=================================================================

enum GameState {
	GS_NULL = 0,
	GS_ATTRACT,
	GS_PLAY_MAIN_CAM,
	GS_PLAY_TPS_CAM,
	GS_COUNT,
	
	GS_PAUSE,
	GS_GAME_OVER,
	GS_MAIN_MENU,
	GS_GAME_SETTINGS,
	GS_GAME_PLAY
};

#endif
