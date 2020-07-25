#pragma once
#include "Tile.h"
#include <vector>

class Grid
{
public:
	Grid(ID3D11Device* _GD);
	int generateAltitude(int x);
	~Grid() = default;

	Tile* getTile(float x, float y);
	Tile* getTile(const Vector2& grid_coords);
	Tile* findTile(const Vector2& world_coords);
	Vector2 findGridCoords(Vector2 world_coords);
	static Vector2 findWorldCoords(const Vector2& grid_coords);
	Vector2 convertToGridCoords(const Vector2& world_coords);

	void draw(DrawData2D* _DD);

	static constexpr int GRID_HEIGHT = 46;
	static constexpr int GRID_WIDTH = 80;
private:
	// I am aware a 2D vector is not optimal, however it allows for
	// simpler code at the cost of memory access optimization
	std::vector<std::vector<Tile>> tiles;
};

