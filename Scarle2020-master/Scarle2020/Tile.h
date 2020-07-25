#pragma once
#include "ImageGO2D.h"

class Tile : public ImageGO2D
{
public:
	Tile(ID3D11Device* _GD, const Vector2& location);
	Tile(const Tile& other) = delete;
	Tile(Tile&& other) noexcept;

	void Draw(DrawData2D* _DD) override;

	void kill();
	[[nodiscard]] Vector2 getCentre();
	[[nodiscard]] bool isAlive();

	static constexpr float TILE_DIMS = 30;
private:
	bool alive = true;
};

