#pragma once
#include "Grid.h"

class PhysicsComponent
{
public:
	PhysicsComponent() = default;
	~PhysicsComponent() = default;

	void setVel(Vector2 vel) { velocity = vel; };
	void setXVel(float _x) { velocity.x = _x; };
	void setYVel(float _y) { velocity.y = _y; };
	void addXVel(float _x) { velocity.x += _x; };
	void addYVel(float _y) { velocity.y += _y; };

	Vector2 move(float dt, Grid* world, const Vector2& startPos);

	Vector2 getVel() { return velocity; };
	bool isGrounded() { return grounded; };

private:
	static constexpr float MV_MAXSPEED = 30;
	static constexpr float MV_PLAYERACCEL = 6;
	static constexpr float MV_PLAYERAIRACCEL = 3;
	static constexpr float MV_FRICTION = 4;
	
	void applyFriction(float dt);
	void checkCollisions(Grid* world);

	Vector2 velocity = Vector2(0, 0);
	bool grounded = true;
	float weight = 10;
};

