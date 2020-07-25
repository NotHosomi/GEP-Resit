#pragma once
#include "Grid.h"
#include "Collider.h"

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
	
	enum TraceDir
	{
		TRACE_N = 0,
		TRACE_NE,
		TRACE_E,
		TRACE_SE,
		TRACE_S,
		TRACE_SW,
		TRACE_W,
		TRACE_NW
	};
	enum CollisionFace
	{
		TOP = 0,
		RIGHT = 1,
		BOTTOM = 2,
		LEFT = 3
	};

	void applyFriction(float dt);
	bool checkCollisionsCheap(Grid* world, const Collider& object, const Vector2& velocity);
	Vector2 checkCollisions(Grid* world, const Collider& object, const Vector2& velocity);
	vector<Tile&> genTileList(Grid* world, const Collider& object, const Vector2& velocity);
	TraceDir genTraceDir(const Vector2& mv_delta);

	bool checkTile(CollisionFace* face, const Collider& incident_object,
		const Vector2& velocity, const Collider& collider);

	Vector2 velocity = Vector2(0, 0);
	bool grounded = true;
	bool bouncy = false;
	float weight = 10;
};

