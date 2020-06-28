#include "pch.h"
#include "TeamManager.h"

void TeamManager::updateTurn(Teams _colour)
{
	switch (_colour)
	{
	case Teams::RED:
		if(m_nextRedTeam == m_teamCount)
			m_nextRedTeam = 1;
		else
			m_nextRedTeam++;
		break;
	case Teams::GREEN:
		if (m_nextGreenTeam == m_teamCount)
			m_nextGreenTeam = 1;
		else
			m_nextGreenTeam++;
		break;
	case Teams::BLUE:
		if (m_nextBlueTeam == m_teamCount)
			m_nextBlueTeam = 1;
		else
			m_nextBlueTeam++;
		break;
	case Teams::PURPLE:
		if (m_nextPurpleTeam == m_teamCount)
			m_nextPurpleTeam = 1;
		else
			m_nextPurpleTeam++;
		break;
	}
}
