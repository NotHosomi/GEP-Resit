#include "pch.h"
#include "Tile.h"

Tile::Tile(ID3D11Device* _GD, const Vector2& location)
{
	SpriteCmp = new ImageGO2D("ground", _GD);
	SpriteCmp->SetPos(location*TILE_DIMS);
}

void Tile::draw(DrawData2D* _DD)
{
	if (alive)
	{
		SpriteCmp->Draw(_DD);
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
