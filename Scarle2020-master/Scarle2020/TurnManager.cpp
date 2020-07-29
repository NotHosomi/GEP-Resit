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
	hud_alert.Draw(_DD);
	hud_alert_subtitle.Draw(_DD);
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
	show_alert = false;
	_TM->getCurrentUnit()->setAwake(true);
	timer = TIME_ACT;
}

void TurnManager::stageFlee(TeamsManager* _TM)
{
	_TM->getCurrentUnit()->setAwake(true);
	timer = TIME_FLEE;
}

void TurnManager::stageWait(TeamsManager* _TM)
{
	_TM->getCurrentUnit()->setAwake(true);
	timer = TIME_POST;
}

void TurnManager::stageEnd(TeamsManager* _TM)
{
	int winner_id = _TM->getCurrentTeamId();
	hud_alert.SetString(ALERT_END);
	hud_alert.SetColour(TeamsManager::colourPicker(winner_id));
	hud_alert_subtitle.SetString(ALERT_SUB_END_1 + to_string(winner_id + 1) + ALERT_SUB_END_2);
	hud_alert_subtitle.SetColour(TeamsManager::colourPicker(winner_id));
	timer = 0;
}
