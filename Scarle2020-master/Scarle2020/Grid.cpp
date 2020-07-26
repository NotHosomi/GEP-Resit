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
		for (int y = altitude; y < GRID_HEIGHT; ++y)
		{
			tiles.back().emplace_back(_GD, Vector2(x, y));
		}
	}
}

int Grid::generateAltitude(int x)
{
#if 0
	// generate altitude using sine wave
	float y = 2 * sin(0.4 * x) + 15;
	return floor(y);
#else
	// generate altitude using a quartic polynomial
	// i.e. two peaks with a valley
	float f = 0.31 * x - 13;
	float y = 0.01 * (f + 3.2) * (f + 8.5) * (f - 7.2) * (f - 2.1) + 20;
	return floor(y);
#endif
}

void Grid::draw(DrawData2D* _DD)
{
	for (auto& column : tiles)
	{
		for (auto& tile : column)
		{
			tile.Draw(_DD);
		}
	}
}

Tile* Grid::getTile(float x, float y)
{
	int x_coord = static_cast<int>(floor(x));
	int y_coord = static_cast<int>(floor(y));
	if (x_coord < 0 || y_coord < 0 ||
		x_coord >= GRID_WIDTH || y_coord >= GRID_HEIGHT)
	{
		return nullptr;
	}
	return &tiles[x_coord][y_coord];
}

Tile* Grid::getTile(const Vector2& grid_coords)
{
	int x_coord = static_cast<int>(floor(grid_coords.x));
	int y_coord = static_cast<int>(floor(grid_coords.y));
	if (x_coord < 0 || y_coord < 0 ||
		x_coord >= GRID_WIDTH || y_coord >= GRID_HEIGHT)
	{
		return nullptr;
	}
	return &tiles[x_coord][y_coord];
}

Tile* Grid::findTile(const Vector2& world_coords)
{
	Vector2 grid_coords = findGridCoords(world_coords);
	return getTile(grid_coords);
}

Vector2 Grid::findGridCoords(Vector2 world_coords)
{
	Vector2 grid_coords;
	grid_coords.x = world_coords.x / Tile::TILE_DIMS;
	grid_coords.y = world_coords.y / Tile::TILE_DIMS;
	grid_coords.x = floor(grid_coords.x);
	grid_coords.y = floor(grid_coords.y);
	return grid_coords;
}

Vector2 Grid::findWorldCoords(const Vector2& grid_coords)
{
	Vector2 world_coords;
	world_coords.x = grid_coords.x * Tile::TILE_DIMS;
	world_coords.y = grid_coords.y * Tile::TILE_DIMS;
	return world_coords;
}

Vector2 Grid::convertToGridCoords(const Vector2& world_coords)
{
	Vector2 out = world_coords;
	out *= (1 / Tile::TILE_DIMS);
	return out;
}
