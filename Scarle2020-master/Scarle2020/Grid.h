#pragma once
#include "Tile.h"
#include <array>

class Grid
{
public:
	Grid(ID3D11Device* _GD);
	~Grid() = default;

	Tile* getTile(int x, int y);
	Tile* getTile(Vector2 pos);

	void draw(DrawData2D* _DD);

	static constexpr int GRID_HEIGHT = 24;
	static constexpr int GRID_WIDTH = 43;
private:
	std::array<std::array<Tile, GRID_WIDTH>, GRID_HEIGHT> tiles;
};

