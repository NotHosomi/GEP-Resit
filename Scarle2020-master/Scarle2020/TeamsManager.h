#pragma once
#include "Unit.h"
#include <vector>
#include <array>
#include <SimpleMath.h>

struct TeamData
{
	vector<Unit> m_worm_list = {};
	int queuedWorm = 0;
	array<int, 4> m_ammo_list = { -1, -1, -1, -1 };
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

	static Color colourPicker(int team_id);
private:
	std::vector<TeamData> m_team_lists = {};
	int m_current_team = 0;
	Unit* current_unit = nullptr;
};

