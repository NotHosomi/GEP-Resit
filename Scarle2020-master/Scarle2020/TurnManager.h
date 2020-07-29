#pragma once
#include "TextGO2D.h"
#include "GameState.h"

class TurnManager
{
public:
	void Tick(GameData* _GD);
	void DrawHud(DrawData2D* _DD);

	enum TurnState {
		GS_NULL = 0,
		GS_ATTRACT,
		GS_TURN_PRE,
		GS_TURN_ACT,
		GS_TURN_FLEE,
		GS_TURN_POST,
		GS_END
	};

private:
	static constexpr float TIME_PRE = 2;
	static constexpr float TIME_ACT = 60;
	static constexpr float TIME_FLEE = 5;
	static constexpr float TIME_POST = 1;

	void nextStage(GameData* _GD);

	TurnManager::TurnState state = GS_TURN_PRE; //global GameState
	float timer = 0;
	TextGO2D hud_timer = TextGO2D("");
	TextGO2D hud_alert = TextGO2D("");
	TextGO2D hud_alert_subtitle = TextGO2D("");
};

