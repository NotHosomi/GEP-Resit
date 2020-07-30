#pragma once
#include "ImageGO2D.h"
#include "Collider.h"
#include "GridReso.h"

class Tile : public ImageGO2D
{
public:
	Tile(ID3D11Device* _GD, const Vector2& location, Color color = COLOUR_DIRT);
	Tile(const Tile& other) = delete;
	Tile(Tile&& other) noexcept;

	void Draw(DrawData2D* _DD) override;

	void kill();
	[[nodiscard]] Vector2 getCentre() const;
	[[nodiscard]] bool isAlive() const;
	[[nodiscard]] Collider generateCollider() const;

#ifdef _GRID_RESO_HIGH
	static constexpr float TILE_DIMS = 12;
#else
	static constexpr float TILE_DIMS = 16;
#endif
	static constexpr Color COLOUR_GRASS = Color(81, 194, 83); // green
	static constexpr Color COLOUR_DIRT = Color(81, 194, 83); // brown
private:
	bool alive = true;
};

