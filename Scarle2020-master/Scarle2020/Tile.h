#pragma once
#include "ImageGO2D.h"

class Tile
{
public:
	Tile(ID3D11Device* _GD, const Vector2& location);

	void draw(DrawData2D* _DD);

	void kill();

	static constexpr float TILE_DIMS = 30;
private:
	bool alive = true;
	ImageGO2D* SpriteCmp = nullptr;
};

