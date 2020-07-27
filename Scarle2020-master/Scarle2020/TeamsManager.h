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
	void createUnit(ID3D11Device* _GD, const Vector2& location, int team_id);
	bool seekNextUnit();

	int GetCurrentTeam();
	Unit* getCurrentUnit();

	void applyDamages();

private:
	std::vector<TeamData> m_team_lists = {};
	int m_current_team = 0;
	Unit* current_unit = nullptr;
};

