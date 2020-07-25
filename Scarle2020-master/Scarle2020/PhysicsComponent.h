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

	Vector2 move(float dt, Vector2 startPos);

	Vector2 getVel() { return velocity; };
	bool isGrounded() { return grounded; };

private:
	static constexpr float MV_MAXSPEED = 30;
	static constexpr float MV_PLAYERACCEL = 30;
	static constexpr float MV_PLAYERAIRACCEL = 30;
	
	void applyFriction();
	void checkCollisions(Grid* world);

	Vector2 velocity = Vector2(0, 0);
	bool grounded = true;
	float weight = 10;
};

