#include "pch.h"
#include "Unit.h"
#include "GameData.h"

Unit::Unit(ID3D11Device* _GD, const Vector2& location) : 
	ImageGO2D("ally", _GD),
	PhysCmp(Vector2(UNIT_WIDTH, UNIT_HEIGHT), UNIT_WEIGHT, UNIT_ELASTICITY)
{
	m_pos = location;
}

void Unit::Tick(GameData* _GD)
{
	PhysCmp.move(_GD->m_dt, _GD->p_world, m_pos);
}