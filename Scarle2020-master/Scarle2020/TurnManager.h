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

	TurnState getState();
	void nextStage(TeamsManager* _TM);
	void setWaiting(bool _wait);
	bool wishNewGame() { return wish_new_game; };
private:
	static constexpr float TIME_PRE = 5;
	static constexpr float TIME_ACT = 60;
	static constexpr float TIME_FLEE = 5;
	static constexpr float TIME_POST = 0.999;
	const Vector2 ALERT_POS = Vector2(400, 10);
	const Vector2 ALERT_SUB_POS = Vector2(400, 60);
	const Vector2 ALERT_SUB_SCALE = Vector2(0.8, 0.8);
	const string ALERT_START = "WORMS!";
	const string ALERT_PRE = "Player ";
	const string ALERT_SUB_PRE = "Press any key";
	const string ALERT_END = "GAME OVER";
	const string ALERT_SUB_END_1 = "Player ";
	const string ALERT_SUB_END_2 = " wins!\n\nPress any key to restart";

	void stagePre(TeamsManager* _TM);
	void stageAct(TeamsManager* _TM);
	void stageFlee(TeamsManager* _TM);
	void stageWait(TeamsManager* _TM);
	void stageEnd(TeamsManager* _TM);

	TurnManager::TurnState state = TS_PRE;
	float timer = 0;
	bool show_alert = true;
	bool waiting_on_projectile = false;
	TextGO2D hud_timer = TextGO2D("");
	TextGO2D hud_alert = TextGO2D("");
	TextGO2D hud_alert_subtitle = TextGO2D("");

	bool wish_new_game = false;
};

