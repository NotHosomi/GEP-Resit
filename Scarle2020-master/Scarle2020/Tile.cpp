#include "pch.h"
#include "Tile.h"
#include "DrawData2D.h"

Tile::Tile(ID3D11Device* _GD, const Vector2& location, Color colour) :
	ImageGO2D("tile", _GD)
{
	SetPos(location * TILE_DIMS);

	SetScale(Vector2(TILE_DIMS, TILE_DIMS));
	// SetColour(colour); // WHY IS THIS NOT WORKING??? Just gonna set the raw sprite to brown in paint :<
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

Vector2 Tile::getCentre() const
{
	// TODO: Convert Tile to child of ImageGO2D
	Vector2 pos = m_pos;
	pos.x += TILE_DIMS / 2;
	pos.y += TILE_DIMS / 2;
	return pos;
}

bool Tile::isAlive() const
{
	return alive;
}

Collider Tile::generateCollider() const
{
	return Collider(m_pos.x, m_pos.y, TILE_DIMS, TILE_DIMS);
}
