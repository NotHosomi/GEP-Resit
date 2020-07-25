#include "pch.h"
#include "Tile.h"
#include "DrawData2D.h"

Tile::Tile(ID3D11Device* _GD, const Vector2& location) :
	ImageGO2D("ground", _GD)
{
	SetPos(location*TILE_DIMS);
}

Tile::Tile(Tile&& other) noexcept : ImageGO2D(std::move(other))
{
	// is this redundant, or do I need to user define it for the ImageGO2D(std::move(other)) call?
	alive = other.alive;
	other.alive = false;
}

void Tile::Draw(DrawData2D* _DD)
{
	if (alive)
	{
		_DD->m_Sprites->Draw(m_pTextureRV, m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
	}
}

void Tile::kill()
{
	alive = false;
}

Vector2 Tile::getCentre()
{
	// TODO: Convert Tile to child of ImageGO2D
	Vector2 pos = m_pos;
	pos.x += TILE_DIMS / 2;
	pos.y += TILE_DIMS / 2;
	return pos;
}

bool Tile::isAlive()
{
	return alive;
}
