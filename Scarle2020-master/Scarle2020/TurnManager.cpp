#include "pch.h"
#include "TurnManager.h"
#include "GameData.h"
#include "TeamsManager.h"
#include <algorithm>

void TurnManager::Tick(GameData* _GD)
{
	if (state == TS_WAIT)
	{
		vector<TeamData*> teams = _GD->m_Teams.getAllTeams();
		// If any worms are not still
		if (std::any_of(teams.begin(), teams.end(),
			[](TeamData* team)
			{
				return std::any_of(team->unit_list.begin(), team->unit_list.end(),
					[](Unit* unit) { return !unit->getPhysCmp()->isStill(); });
			}))
		{
			// Note: would've rather done "If not all units are still", but std::all_of() was not being recognised
			// TODO: Check if ALL gameobjects are still, not just units
			return;
		}
	}
	if (state == TS_END)
	{
		if (_GD->m_Input.any())
		{
			// TODO: Reset game
			_GD->m_Input.clear();
		}
		return;
	}
	if (state == TS_PRE)
	{
		if (_GD->m_Input.any())
		{
			timer = 0;
			_GD->m_Input.clear();
		}
	}

	timer -= _GD->m_dt;
	if (timer <= 0)
	{
		nextStage(&_GD->m_Teams);
	}
	// update HUD timer
	auto rounded_tmr = static_cast<int>(floor(timer));
	hud_timer.SetString(to_string(rounded_tmr));
}

void TurnManager::DrawHud(DrawData2D* _DD)
{
	hud_timer.Draw(_DD);
	if (show_alert)
	{
		hud_alert.Draw(_DD);
		hud_alert_subtitle.Draw(_DD);
	}
}

void TurnManager::init(GameData* _GD)
{
	state = TS_PRE;
	timer = 60;
	show_alert = true;
	_GD->m_Teams.getCurrentUnit();

	int team_id = _GD->m_Teams.getCurrentTeamId();
	hud_alert.SetPos(ALERT_POS);
	hud_alert.SetString(ALERT_START);
	hud_alert.SetColour(TeamsManager::colourPicker(team_id));
	hud_alert_subtitle.SetPos(ALERT_SUB_SCALE);
	hud_alert_subtitle.SetScale(0.8);
	hud_alert_subtitle.SetString(ALERT_SUB_PRE);
	hud_alert_subtitle.SetColour(TeamsManager::colourPicker(team_id));
}

void TurnManager::nextStage(TeamsManager* _TM)
{
	switch (state)
	{
	case TS_PRE: stageAct(_TM);
		break;
	case TS_ACT: stageFlee(_TM);
		break;
	case TS_FLEE: stageWait(_TM);
		break;
	case TS_WAIT: stagePre(_TM);
		break;
	}
}

void TurnManager::stagePre(TeamsManager* _TM)
{
	state = TS_PRE;
	timer = TIME_PRE;
	show_alert = true;
	bool gameOver = false;
	gameOver = _TM->seekNextUnit();
	if (gameOver)
	{
		stageEnd(_TM);
	}
	// update Hud alert
	int team_id = _TM->getCurrentTeamId();
	hud_alert.SetString(ALERT_PRE);
	hud_alert.SetColour(TeamsManager::colourPicker(team_id));
	hud_alert_subtitle.SetString(ALERT_SUB_PRE);
	hud_alert_subtitle.SetColour(TeamsManager::colourPicker(team_id));
}

void TurnManager::stageAct(TeamsManager* _TM)
{
	state = TS_ACT;
	timer = TIME_ACT;
	show_alert = false;
	_TM->getCurrentUnit()->setAwake(true);
}

void TurnManager::stageFlee(TeamsManager* _TM)
{
	state = TS_FLEE;
	timer = TIME_FLEE;
}

void TurnManager::stageWait(TeamsManager* _TM)
{
	state = TS_WAIT;
	timer = TIME_POST;
	_TM->getCurrentUnit()->setAwake(false);
}

void TurnManager::stageEnd(TeamsManager* _TM)
{
	state = TS_END;
	timer = 0;
	int winner_id = _TM->getCurrentTeamId();
	hud_alert.SetString(ALERT_END);
	hud_alert.SetColour(TeamsManager::colourPicker(winner_id));
	hud_alert_subtitle.SetString(ALERT_SUB_END_1 + to_string(winner_id + 1) + ALERT_SUB_END_2);
	hud_alert_subtitle.SetColour(TeamsManager::colourPicker(winner_id));
}
