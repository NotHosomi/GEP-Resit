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
