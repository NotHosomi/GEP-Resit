#pragma once
#include "Team.h"
class PlayerComponent
{
public:
	PlayerComponent() = default;
	~PlayerComponent() = default;

	void setTeam(Teams _team, int _id);
	Teams getTeam() { return m_team; }
	Team* getTeamStruct() { return &m_teamID; }

	int health() { return m_health; }
	void health(int _value) { m_health = _value; }

	void damage(int _value) { m_health -= _value; }
private:
	int m_health = 0;
	Teams m_team;
	Team m_teamID;
};

