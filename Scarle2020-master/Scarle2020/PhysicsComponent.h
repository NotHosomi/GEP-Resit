#pragma once
#include "Grid.h"
#include "Collider.h"

class PhysicsComponent
{
public:
	PhysicsComponent() = default;
	PhysicsComponent(Vector2 dimensions, float _weight, float _elasticity);
	~PhysicsComponent() = default;
	void setVel(Vector2 vel) { velocity = vel; };
	void setXVel(float _x) { velocity.x = _x; };
	void setYVel(float _y) { velocity.y = _y; };
	void addVel(Vector2 vel) { velocity += vel; };
	void addXVel(float _x) { velocity.x += _x; };
	void addYVel(float _y) { velocity.y += _y; };
	void setGrounded(bool _grounded) { grounded = _grounded; };

	void move(float dt, Grid* world, Vector2& pos);

	Vector2 getVel() { return velocity; };
	bool isGrounded() { return grounded; };

private:
	static constexpr float MV_MAXSPEED = 10000;
	static constexpr float MV_FRICTION = 10;
	static constexpr float MV_SKIN = 0.05; // forced gap between objects that have collided

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
		F_NONE = 0,
		F_TOP = 1,
		F_RIGHT = 2,
		F_BOTTOM = 4,
		F_LEFT = 8
	};

	void checkGrounded(Grid* world, const Collider& object);
	void applyFriction(float dt);
	bool checkCollisionsCheap(Grid* world, const Collider& object, const Vector2& velocity);

	Vector2 checkCollisions(Grid* world, const Collider& object, Vector2& velocity);
	vector<Vector2> genOrigList(const Collider& object);
	vector<Vector2> genDestList(const vector<Vector2>& orig_list, const Vector2& velocity);
	vector<Tile*> genTileList(Grid* world, const vector<Vector2>& dest_list);
	vector<Tile*> genTileList(Grid* world, const Collider& object, const Vector2& velocity);
	TraceDir genTraceDir(const Vector2& mv_delta);

	Vector2 complexTrace(vector<Vector2> orig_list, Vector2& velocity,
		vector<Tile*> tile_list, PhysicsComponent::TraceDir dir);
	float vertexProject(const Vector2& mv_delta, const Vector2& origin, Collider otherHitbox,
		PhysicsComponent::TraceDir T_dir, CollisionFace& OUT_face);
	bool isCoordUnderVector(const Vector2& origin, const Vector2& delta, Vector2 coord);

	bool checkTile(const Tile* tile) const;

	Collider self;
	Vector2 velocity = Vector2(0, 0);
	bool grounded = false;
	bool bouncy = false;
	float elasticity = 0.1;
	float weight = 100;
};

