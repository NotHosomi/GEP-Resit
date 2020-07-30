#include "pch.h"
#include "Grid.h"
#include <random>

Grid::Grid(ID3D11Device* _GD)
{
	// generate terrain
	seedTerrain();

	tiles.reserve(GRID_WIDTH);
	for (int x = 0; x < GRID_WIDTH; ++x)
	{
		tiles.emplace_back(std::vector<Tile>());
		tiles.back().reserve(GRID_HEIGHT);
		int altitude = generateAltitude(x);
		// sanity checks
		if (altitude > GRID_HEIGHT)
		{
			altitude = GRID_HEIGHT;
		}
		if (altitude < 0)
		{
			altitude = 0;
		}
		// spawn tiles
		for (int y = 0; y < altitude; ++y)
		{
			tiles.back().emplace_back(_GD, Vector2(x, y));
			tiles.back().back().kill();
		}
		for (int y = altitude; y < GRID_HEIGHT; ++y)
		{
			if (y == altitude)
				tiles.back().emplace_back(_GD, Vector2(x, y), Tile::COLOUR_GRASS);
			else
				tiles.back().emplace_back(_GD, Vector2(x, y));
		}
	}
}

int Grid::generateAltitude(int x)
{
#ifndef _GRID_RESO_HIGH
	// generate altitude using a quartic polynomial
	// i.e. two peaks with a valley
	float f = 0.23 * x - 13;
	float y = 0.01 * (f + 3.2) * (f + 8.5) * (f - 7.2) * (f - 2.1) + 20;
#else
	// generate altitude using a quartic polynomial
	// i.e. two peaks with a valley
	// float f = 0.23 * x - 12;
	// float y = 0.008 * (f + 5.7) * (f + 6.5) * (f - 8.7) * (f - 3.1) + 25;

	// alternative map
	// float y = 0.01 * (f + 5.9) * (f + 6.7) * (f - 8.7) * (f - 5.8) + 20;

	float f = 0.22 * x - 12;
	float y = q_m * (f + q_a) * (f + q_b) * (f + q_c) * (f + q_d) + q_e;
#endif
	return floor(y);
}

void Grid::seedTerrain()
{
	std::default_random_engine re{ std::random_device{}() };
	std::uniform_int_distribution<int> dist{ 0, 100 };
	q_m = dist(re);
	q_m = q_m * (GRID_QUARTIC_M_MAX - GRID_QUARTIC_M_MIN) / 100 + GRID_QUARTIC_M_MIN;
	q_a = dist(re);
	q_a = q_a * (GRID_QUARTIC_A_MAX - GRID_QUARTIC_A_MIN) / 100 + GRID_QUARTIC_A_MIN;
	q_b = dist(re);
	q_b = q_b * (GRID_QUARTIC_B_MAX - GRID_QUARTIC_B_MIN) / 100 + GRID_QUARTIC_B_MIN;
	q_c = dist(re);
	q_c = q_c * (GRID_QUARTIC_C_MAX - GRID_QUARTIC_C_MIN) / 100 + GRID_QUARTIC_C_MIN;
	q_d = dist(re);
	q_d = q_d * (GRID_QUARTIC_D_MAX - GRID_QUARTIC_D_MIN) / 100 + GRID_QUARTIC_D_MIN;
	q_e = dist(re);
	q_e /= 100;
	q_e *= GRID_QUARTIC_E_MAX - GRID_QUARTIC_E_MIN;
	q_e += GRID_QUARTIC_E_MIN;
	//q_e = q_e * (GRID_QUARTIC_E_MAX - GRID_QUARTIC_E_MIN) / 100 + GRID_QUARTIC_E_MIN;
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

// Note: Does not round output, if you want rounded output use findGridCoords()
Vector2 Grid::convertToGridCoords(const Vector2& world_coords)
{
	Vector2 out = world_coords;
	out /= Tile::TILE_DIMS;
	return out;
}

vector<Tile*> Grid::getTilesInRadius(Vector2 origin, float radius)
{
	vector<Tile*> tile_list;

	for (auto& column : tiles)
	{
		for (auto& tile : column)
		{
			Vector2 diff = tile.GetPos() - origin;
			if (diff.Length() < radius)
			{
				tile_list.emplace_back(&tile);
			}
		}
	}

	return tile_list;
}
