#pragma once
#include "TextGO2D.h"
#include "GameState.h"

class TeamsManager;

class TurnManager
{
public:
	void Tick(GameData* _GD);
	void DrawHud(DrawData2D* _DD);
	void init(GameData* _GD);

	enum TurnState {
		TS_PRE,
		TS_ACT,
		TS_FLEE,
		TS_WAIT,
		TS_END
	};

	// TODO init??
	// TODO integrate with Gamecpp
private:
	static constexpr float TIME_PRE = 2;
	static constexpr float TIME_ACT = 60;
	static constexpr float TIME_FLEE = 5;
	static constexpr float TIME_POST = 0.999;
	const string ALERT_START = "WORMS!";
	const string ALERT_PRE = "Player ";
	const string ALERT_SUB_PRE = "Press any key";
	const string ALERT_END = "GAME OVER";
	const string ALERT_SUB_END_1 = "Player ";
	const string ALERT_SUB_END_2 = " wins!";

	void nextStage(TeamsManager* _TM);

	void stagePre(TeamsManager* _TM);
	void stageAct(TeamsManager* _TM);
	void stageFlee(TeamsManager* _TM);
	void stageWait(TeamsManager* _TM);
	void stageEnd(TeamsManager* _TM);

	TurnManager::TurnState state = TS_PRE; //global GameState
	float timer = 0;
	bool show_alert = true;
	TextGO2D hud_timer = TextGO2D("");
	TextGO2D hud_alert = TextGO2D("");
	TextGO2D hud_alert_subtitle = TextGO2D("");
};

