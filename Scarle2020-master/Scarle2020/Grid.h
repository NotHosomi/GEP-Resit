#pragma once
#include "GridReso.h"
#include "Tile.h"
#include <vector>

class Grid
{
public:
	Grid(ID3D11Device* _GD);
	~Grid() = default;

	Tile* getTile(float x, float y);
	Tile* getTile(const Vector2& grid_coords);
	Tile* findTile(const Vector2& world_coords);
	Vector2 findGridCoords(Vector2 world_coords);
	static Vector2 findWorldCoords(const Vector2& grid_coords);
	Vector2 convertToGridCoords(const Vector2& world_coords);

	vector<Tile*> getTilesInRadius(Vector2 origin, float radius);

	void draw(DrawData2D* _DD);
	Vector2 genSpawnCoord();

#ifdef _GRID_RESO_HIGH
	static constexpr int GRID_HEIGHT = 61;
	static constexpr int GRID_WIDTH = 106;
#else
	static constexpr int GRID_HEIGHT = 46;
	static constexpr int GRID_WIDTH = 80;
#endif
private:
#ifdef _GRID_RESO_HIGH
	static constexpr float GRID_QUARTIC_M_MIN = 0.01;
	static constexpr float GRID_QUARTIC_M_MAX = 0.015;
	static constexpr float GRID_QUARTIC_A_MIN = 5;
	static constexpr float GRID_QUARTIC_A_MAX = 5;
	static constexpr float GRID_QUARTIC_B_MIN = 5;
	static constexpr float GRID_QUARTIC_B_MAX = 7.5;
	static constexpr float GRID_QUARTIC_C_MIN = -7;
	static constexpr float GRID_QUARTIC_C_MAX = -6;
	static constexpr float GRID_QUARTIC_D_MIN = -8;
	static constexpr float GRID_QUARTIC_D_MAX = -0.5;
	static constexpr float GRID_QUARTIC_E_MIN = 24;
	static constexpr float GRID_QUARTIC_E_MAX = 26;
#endif
	int generateAltitude(int x);
	void seedTerrain();

	// quartic components
	float q_m = 0;
	float q_a = 0;
	float q_b = 0;
	float q_c = 0;
	float q_d = 0;
	float q_e = 0;

	// I am aware a 2D vector is not optimal, however it allows for
	// simpler code at the cost of memory access optimization
	std::vector<std::vector<Tile>> tiles;

	std::vector<int> unit_spawns;
};

