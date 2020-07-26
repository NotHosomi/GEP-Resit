#pragma once

class Collider {
public:
	Collider() = default;
	Collider(float _x, float _y, float _w, float _h);
	float x = 0;
	float y = 0;
	float width = 0;
	float height = 0;
	[[nodiscard]] bool checkOverlap(float x, float y) const;
	[[nodiscard]] bool checkOverlap(const Collider& rhs) const;
private:
	[[nodiscard]] bool isBetween(float value, float min, float max) const;
};