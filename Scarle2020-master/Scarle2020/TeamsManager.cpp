#include "pch.h"
#include "TeamsManager.h"
#include <algorithm>

//TeamsManager::~TeamsManager()
//{
//	for (auto& team : m_team_lists)
//	{
//		for (int i = 0; i < team.m_worm_list.size(); ++i)
//		{
//	
//		}
//	}
//}

// Consumes the Unit ptr
void TeamsManager::createUnit(ID3D11Device* _GD, const Vector2& location, int team_id)
{
	while (team_id >= m_team_lists.size())
	{
		m_team_lists.push_back(TeamData());
	}
	// Add the worm pointer to the team's worm list
	m_team_lists[team_id].m_worm_list.emplace_back(_GD, location, team_id);
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

		for (auto& unit : m_team_lists[m_current_team].m_worm_list)
		{
			if (unit.isAlive())
			{
				found_team = false;;
			}
			else
			{
				m_current_team++;
			}
		}
	} while (!found_team);

	TeamData* current_team = &m_team_lists[m_current_team];
	Unit* thisWorm;
	for (int i = 0; i < current_team->m_worm_list.size(); ++i)
	{
		current_team->queuedWorm++;
		if (current_team->queuedWorm >= current_team->m_worm_list.size())
		{
			current_team->queuedWorm = 0;
		}
		thisWorm = &current_team->m_worm_list[current_team->queuedWorm];
		if (thisWorm->isAlive() > 0)
		{
			break;
		}
	}
	current_unit = thisWorm;
	return true;
}

int TeamsManager::GetCurrentTeam()
{
	return m_current_team;
}

Unit* TeamsManager::getCurrentUnit()
{
	return current_unit;
}

// call once all units have stopped moving
void TeamsManager::applyDamages()
{
	for (auto& team : m_team_lists)
	{
		for (auto& unit : team.m_worm_list)
		{
			unit.applyDamages();
		}
	}
}
