#pragma once
#include "Grid.h"
#include "Collider.h"

class PhysicsComponent
{
public:
	PhysicsComponent(Vector2 dimensions, float _weight);
	~PhysicsComponent() = default;
	void setVel(Vector2 vel) { velocity = vel; };
	void setXVel(float _x) { velocity.x = _x; };
	void setYVel(float _y) { velocity.y = _y; };
	void addVel(Vector2 vel) { velocity += vel; };
	void addXVel(float _x) { velocity.x += _x; };
	void addYVel(float _y) { velocity.y += _y; };

	void move(float dt, Grid* world, Vector2& pos);

	Vector2 getVel() { return velocity; };
	bool isGrounded() { return grounded; };

private:
	static constexpr float MV_MAXSPEED = 30;
	static constexpr float MV_PLAYERACCEL = 6;
	static constexpr float MV_PLAYERAIRACCEL = 3;
	static constexpr float MV_FRICTION = 4;
	Collider self;

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
		NONE = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 4,
		LEFT = 8
	};

	void applyFriction(float dt);
	bool checkCollisionsCheap(Grid* world, const Collider& object, const Vector2& velocity);
	Vector2 checkCollisions(Grid* world, const Collider& object, const Vector2& velocity);
	vector<Vector2> genOrigList(const Collider& object);
	vector<Vector2> genDestList(const vector<Vector2>& orig_list, const Vector2& velocity);
	vector<Tile*> genTileList(Grid* world, const vector<Vector2>& dest_list);
	vector<Tile*> genTileList(Grid* world, const Collider& object, const Vector2& velocity);
	TraceDir genTraceDir(const Vector2& mv_delta);

	void complexTrace(vector<Vector2> orig_list, Vector2& velocity,
		vector<Tile*> tile_list, PhysicsComponent::TraceDir dir);

	bool vertexProject(Vector2& mv_delta, Vector2 origin,
		Collider otherHitbox, PhysicsComponent::TraceDir T_dir);
	bool isCoordUnderVector(const Vector2& origin, const Vector2& delta, Vector2 coord);

	Vector2 velocity = Vector2(0, 0);
	bool grounded = false;
	bool bouncy = false;
	float weight;
};

