#pragma once
#include "Unit.h"
#include <vector>
#include <array>
#include <SimpleMath.h>

struct TeamData
{
	vector<Unit*> unit_list = {};
	int queuedWorm = 0;
	array<int, 6> ammo_list = { -1, 5, 5, 2, 3, 1 }; // -1 means infinite ammo
};

class TeamsManager
{
public:
	TeamsManager() = default;
	~TeamsManager() = default;
	//Unit* createUnit(ID3D11Device* _GD, const Vector2& location, int team_id);
	void init();

	void addUnitToTeam(Unit* unit);
	bool seekNextUnit();

	int getCurrentTeamId();
	TeamData* getCurrentTeam();
	vector<TeamData*> getAllTeams();
	Unit* getCurrentUnit();
	int ammoCount(int wep_slot);
	void consumeAmmo(int wep_slot);

	void applyDamages(GameData* _GD);

	static Color colourPicker(int team_id);

	static constexpr int NUM_PLAYERS = 4;
	static constexpr int NUM_UNITS_PER_TEAM = 4;
private:
	std::vector<TeamData> m_team_lists = {};
	int m_current_team = 0;
	Unit* current_unit = nullptr;
};

