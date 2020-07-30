#include "pch.h"
#include "TeamsManager.h"
#include <algorithm>
#include <SimpleMath.h>

// Cut - Create unit
#if 0
Unit* TeamsManager::createUnit(ID3D11Device* _GD, const Vector2& location, int team_id)
{
	while (team_id >= m_team_lists.size())
	{
		m_team_lists.push_back(TeamData());
	}
	// Add the worm pointer to the team's worm list
	m_team_lists[team_id].m_worm_list.emplace_back(_GD, location, team_id);
	return &m_team_lists[team_id].m_worm_list.back();
}
#endif

void TeamsManager::init()
{
	//m_current_team = -1;
	//seekNextUnit();
	current_unit = m_team_lists[0].unit_list[0];
	current_unit->setAwake(true);
}

void TeamsManager::addUnitToTeam(Unit* unit)
{
	int team = unit->getTeam();
	while (team >= m_team_lists.size())
	{
		m_team_lists.push_back(TeamData());
	}
	// Add the worm pointer to the team's worm list
	m_team_lists[team].unit_list.emplace_back(unit);
}

// Fetch the next worm to play
// Returns false if no unit found - then m_current_team is the winner
bool TeamsManager::seekNextUnit()
{
	int start_team = m_current_team;
	// Search for next alive 
	m_current_team++;
	bool found_team = false;
	do
	{
		if (m_current_team == m_team_lists.size())
		{
			m_current_team = 0;
		}
		if (m_current_team == start_team)
		{
			// THIS TEAM HAS WON
			return true;
		}

		for (auto& unit : m_team_lists[m_current_team].unit_list)
		{
			if (unit->isAlive())
			{
				found_team = true;
			}
			else
			{
				m_current_team++;
			}
		}
	} while (!found_team);

	TeamData* current_team = &m_team_lists[m_current_team];
	Unit* thisWorm;
	for (int i = 0; i < current_team->unit_list.size(); ++i)
	{
		current_team->queuedWorm++;
		if (current_team->queuedWorm >= current_team->unit_list.size())
		{
			current_team->queuedWorm = 0;
		}
		thisWorm = current_team->unit_list[current_team->queuedWorm];
		if (thisWorm->isAlive() > 0)
		{
			break;
		}
	}
	current_unit = thisWorm;
	return false;
}

int TeamsManager::getCurrentTeamId()
{
	return m_current_team;
}

TeamData* TeamsManager::getCurrentTeam()
{
	return &m_team_lists[m_current_team];
}

vector<TeamData*> TeamsManager::getAllTeams()
{
	vector<TeamData*> teams;
	for (auto& team : m_team_lists)
	{
		teams.emplace_back(&team);
	}
	return teams;
}

Unit* TeamsManager::getCurrentUnit()
{
	return current_unit;
}

int TeamsManager::ammoCount(int wep_slot)
{
	if (wep_slot > m_team_lists[m_current_team].ammo_list.size())
	{
		return 0;
	}
	if (wep_slot < 0)
	{
		return 0;
	}
	return m_team_lists[m_current_team].ammo_list[wep_slot];
}

void TeamsManager::consumeAmmo(int wep_slot)
{
	if (wep_slot > m_team_lists[m_current_team].ammo_list.size())
	{
		return;
	}
	if (wep_slot < 0)
	{
		return;
	}
	if (m_team_lists[m_current_team].ammo_list[wep_slot] <= 0)
	{
		return;
	}
	m_team_lists[m_current_team].ammo_list[wep_slot]--;
}

// call once all units have stopped moving
void TeamsManager::applyDamages(GameData* _GD)
{
	for (auto& team : m_team_lists)
	{
		for (auto& unit : team.unit_list)
		{
			unit->applyDamages(_GD);
		}
	}
}

Color TeamsManager::colourPicker(int team_id)
{
	switch (team_id)
	{
	case 0: return Color((float*)&Colors::Red);
	case 1: return Color((float*)&Colors::Blue);
	case 2: return Color((float*)&Colors::Pink);
	case 3: return Color((float*)&Colors::Yellow);
	default: return Color((float*)&Colors::White);
	}
}
