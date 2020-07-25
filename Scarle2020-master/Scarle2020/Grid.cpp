#include "pch.h"
#include "Grid.h"

Grid::Grid(ID3D11Device* _GD)
{ // TODO array construction? Use vector maybe
	// generate terrain
	tiles.reserve(GRID_WIDTH);
	for (int x = 0; x < GRID_WIDTH; ++x)
	{
		tiles.emplace_back(std::vector<Tile>());
		tiles.back().reserve(GRID_HEIGHT);
		int altitude = generateAltitude(x);

		// spawn tiles
		for (int y = 0; y < altitude; ++y)
		{
			tiles.back().emplace_back(_GD, Vector2(x, y));
			tiles.back().back().kill();
		}
		for (int y = 0; y < altitude; ++y)
		{
			tiles.back().emplace_back(_GD, Vector2(x, y));
		}
	}
}

int Grid::generateAltitude(int x)
{
	// generate altitude using sine wave
	// int altitude = 2 * sin(0.4 * i);
	// altitude -= 15;

	// generate altitude using a quartic polynomial
	// i.e. two peaks with a valley
	float f = 0.5 * x - 10;
	return -0.01 * (f + 3.2) * (f + 8.5) * (f - 7.2) * (f - 2.1) + 10;
}

void Grid::draw(DrawData2D* _DD)
{
	for (auto& column : tiles)
	{
		for (auto& tile : column)
		{
			tile.draw(_DD);
		}
	}
}

Tile* Grid::getTile(float x, float y)
{
	int x_coord = static_cast<int>(floor(x));
	int y_coord = static_cast<int>(std::floor(y));
	return &tiles[x_coord][x_coord];
}

Tile* Grid::getTile(const Vector2& grid_coords)
{
	int x_coord = static_cast<int>(std::floor(grid_coords.x));
	int y_coord = static_cast<int>(std::floor(grid_coords.y));
	return &tiles[x_coord][x_coord];
}

Vector2 Grid::findGridCoords(Vector2 screen_coords)
{
	Vector2 grid_coords;
	grid_coords.x = screen_coords.x / Tile::TILE_DIMS;
	grid_coords.y = screen_coords.y / Tile::TILE_DIMS;
	grid_coords.x = std::floor(grid_coords.x);
	grid_coords.y = std::floor(grid_coords.y);
	return grid_coords;
}

Vector2 Grid::findWorldCoords(const Vector2& tile_coords)
{
	Vector2 world_coords;
	world_coords.x = tile_coords.x * Tile::TILE_DIMS;
	world_coords.y = tile_coords.y * Tile::TILE_DIMS;
	return world_coords;
}

Vector2 Grid::findGridCoords(const Vector2& world_coords)
{
	Vector2 out = world_coords;
	out *= (1 / Tile::TILE_DIMS);
	return out;
}
