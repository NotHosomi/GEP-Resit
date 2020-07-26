#include "pch.h"
#include "Tile.h"
#include "DrawData2D.h"

Tile::Tile(ID3D11Device* _GD, const Vector2& location) :
	ImageGO2D("ground", _GD)
{
	SetPos(location*TILE_DIMS);
	// For some reason, Scarle2020 is displaying a 30x30 sprite as 10x10, so scale it up
	SetScale(TILE_DIMS/10);
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
