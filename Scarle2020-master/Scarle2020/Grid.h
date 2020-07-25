#pragma once
#include "Tile.h"
#include <array>

class Grid
{
public:
	Grid(ID3D11Device* _GD);
	~Grid() = default;

	Tile* getTile(float x, float y);
	Tile* getTile(const Vector2& grid_coords);
	Vector2 findGridCoords(Vector2 screen_coords);
	static Vector2 findWorldCoords(const Vector2& tile_coords);
	Vector2 findGridCoords(const Vector2& world_coords);

	void draw(DrawData2D* _DD);

	static constexpr int GRID_HEIGHT = 24;
	static constexpr int GRID_WIDTH = 43;
private:
	std::array<std::array<Tile, GRID_WIDTH>, GRID_HEIGHT> tiles;
};

