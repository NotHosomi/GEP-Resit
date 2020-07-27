#pragma once
#include "Unit.h"
#include <vector>

struct TeamData
{
	std::vector<Unit> m_worm_list = {};
	int queuedWorm = 0;
};

class TeamsManager
{
public:
	TeamsManager() = default;
	~TeamsManager() = default;
	void AddToTeamList(int team_id, Unit& new_unit);
	bool seekNextUnit();

	int GetCurrentTeam();
	Unit* getCurrentUnit();

	void applyDamages();

private:
	std::vector<TeamData> m_team_lists = {};
	int m_current_team = 0;
	Unit* current_unit = nullptr;
};

