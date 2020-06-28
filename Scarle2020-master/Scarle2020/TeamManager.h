#pragma once
#include "Team.h"
class TeamManager
{
public:
	TeamManager() = default;
	~TeamManager() = default;
	void setTeamCount(int _count) { m_teamCount = _count; };

	int getTeamCount() { return m_teamCount; }
	int getNextRed() { return m_nextRedTeam; }
	int getNextGreen() { return m_nextGreenTeam; }
	int getNextBlue() { return m_nextBlueTeam; }
	int getNextPurple() { return m_nextPurpleTeam; }

	void updateTurn(Teams _colour);
private:
	int m_teamCount = 0;
	//SIMON:: HARD CODED TEAMS, ugh! I think a data structure and just make the teams you need, 
	//also allows for ANY number odf teams.
	int m_nextRedTeam = 1;
	int m_nextGreenTeam = 1;
	int m_nextBlueTeam = 1;
	int m_nextPurpleTeam = 1;
};

