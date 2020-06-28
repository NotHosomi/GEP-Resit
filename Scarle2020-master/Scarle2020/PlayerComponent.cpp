#include "pch.h"
#include "PlayerComponent.h"

void PlayerComponent::setTeam(Teams _team, int _id)
{
	m_team = _team;
	m_teamID.colour = _team;
	m_teamID.id = _id;
}
