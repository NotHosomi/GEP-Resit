#include "pch.h"
#include "Explosion.h"
#include "GameData.h"

Explosion::Explosion(ID3D11Device* _GD, Vector2 position, float _radius, float _damage) :
	ImageGO2D("explosion", _GD)
{
	m_pos = position;
	radius = _radius;
	m_scale = Vector2(radius, radius) / 100; // src sprite is 200x200

	damage = _damage;
}

void Explosion::Tick(GameData* _GD)
{
	vector<Tile*> tiles = _GD->p_World->getTilesInRadius(m_pos, radius);
	for (auto& tile : tiles)
	{
		tile->kill();
	}
	for (auto& team : _GD->m_Teams.getAllTeams())
	{
		for (auto& unit : team->unit_list)
		{
			Vector2 diff = unit->GetPos() - m_pos;
			if (diff.Length() > radius)
			{
				// This unit isn't in the explosion
				continue;
			}
			// quadratic explosion damage falloff
			float falloff = diff.Length() / radius;
			float dmg = damage;
			dmg *= -(falloff * falloff) + 1;
			unit->addDamage(dmg);
			//temp
			unit->applyDamages(); // Why ain't this workin chief

			// linear knockback falloff
			diff.Normalize();
			diff *= damage * falloff * EXP_KNOCKBACK_MULTIPLIER;
			unit->getPhysCmp()->addVel(diff);
		}
	}
	_GD->deletion_list.emplace_back(this);
}


