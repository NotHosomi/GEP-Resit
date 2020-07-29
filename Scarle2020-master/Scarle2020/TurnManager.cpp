#include "pch.h"
#include "TurnManager.h"

void TurnManager::Tick(GameData* _GD)
{
	if (timer < 0)
	{
		nextStage(_GD);
	}
}

void TurnManager::DrawHud(DrawData2D* _DD)
{
	hud_timer.Draw(_DD);
	hud_alert.Draw(_DD);
	hud_alert_subtitle.Draw(_DD);
}
void TurnManager::nextStage(GameData* _GD)
{
	switch (state)
	{

	}
}
