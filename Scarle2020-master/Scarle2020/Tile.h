#pragma once
#include "ImageGO2D.h"
#include "Collider.h"

class Tile : public ImageGO2D
{
public:
	Tile(ID3D11Device* _GD, const Vector2& location);
	Tile(const Tile& other) = delete;
	Tile(Tile&& other) noexcept;

	void Draw(DrawData2D* _DD) override;

	void kill();
	[[nodiscard]] Vector2 getCentre() const;
	[[nodiscard]] bool isAlive() const;
	[[nodiscard]] Collider generateCollider() const;

	static constexpr float TILE_DIMS = 16;
private:
	bool alive = true;
};

