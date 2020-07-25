#include "pch.h"
#include "PhysicsComponent.h"

Vector2 PhysicsComponent::move(float dt, Grid* world, const Vector2& startPos)
{
	if (grounded)
	{
		applyFriction(dt);
	}
	else
	{
		velocity.y += weight * dt;
	}

	Vector2 pos = startPos;
	pos += velocity * dt;
	checkCollisions(world);

	return Vector2();
}

void PhysicsComponent::applyFriction(float dt)
{
	float speed = velocity.Length();
	if (speed == 0)
	{
		return;
	}
	float drop = speed * MV_FRICTION * dt;

	float newspeed = speed - drop;
	if (newspeed < 0)
		newspeed = 0;
	newspeed /= speed;

	velocity *= newspeed;
}

bool PhysicsComponent::checkCollisionsCheap(Grid* world, const Collider& object, const Vector2& velocity)
{
	vector<Tile&> tile_list = genTileList(world, object, velocity);
	return std::any_of(tile_list.begin(), tile_list.end(), [](const Tile& tile)
	{
		return tile.isAlive();
	});
}

Vector2 PhysicsComponent::checkCollisions(Grid* world, const Collider& object, const Vector2& velocity)
{
	Vector2 output = velocity;
	vector<Tile&> tile_list = genTileList(world, object, velocity);
	TraceDir dir = genTraceDir(velocity);

	switch (dir)
	{
	case TRACE_N:
		if (tile_list[0].isAlive() || tile_list[4].isAlive())
			if(output.y < 0)
				output.y = 0;
		break;
	case TRACE_S:
		if (tile_list[3].isAlive() || tile_list[6].isAlive())
			if (output.y > 0)
			{
				if (bouncy)
				{
					grounded = true;
					output.y = 0;
				}
				else
				{
					grounded = true;
					output.y *= -0.5;
				}
			}
		break;
	case TRACE_W:
		if (tile_list[0].isAlive() || tile_list[1].isAlive() || tile_list[2].isAlive())
			if (output.x > 0)
			{
				if (grounded)
					output.x = 0;
				else
					output.x *= -0.5;
			}
		break;
	case TRACE_E:
		if (tile_list[4].isAlive() || tile_list[5].isAlive() || tile_list[6].isAlive())
			if (output.x > 0)
			{
				if (grounded)
					output.x = 0;
				else
					output.x *= -0.5;
			}
		break;
	}
}

vector<Tile&> PhysicsComponent::genTileList(Grid* world, const Collider& object, const Vector2& velocity)
{
	vector<Tile&> tile_list;
	Vector2 checkCoord;
	// Upper Left
	checkCoord.x = object.x + velocity.x;
	checkCoord.y = object.y + velocity.y;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Middle Left
	checkCoord.x = object.x + velocity.x;
	checkCoord.y = object.y + velocity.y + object.height / 2;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Bottom Left
	checkCoord.x = object.x + velocity.x;
	checkCoord.y = object.y + velocity.y + object.height;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Upper Right
	checkCoord.x = object.x + velocity.x;
	checkCoord.y = object.y + velocity.y;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Middle Right
	checkCoord.x = object.x + velocity.x;
	checkCoord.y = object.y + velocity.y + object.height / 2;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Bottom Right
	checkCoord.x = object.x + velocity.x;
	checkCoord.y = object.y + velocity.y + object.height;
	tile_list.emplace_back(world->findTile(checkCoord));
	return tile_list;
}

PhysicsComponent::TraceDir PhysicsComponent::genTraceDir(const Vector2& mv_delta)
{
	TraceDir T_dir = TRACE_N;
	if (mv_delta.x > 0)
	{
		// EAST
		if (mv_delta.y == 0)
		{
			T_dir = TRACE_E;
		}
		// SOUTH EAST
		else if (mv_delta.y < 0)
		{
			T_dir = TRACE_SE;
		}
		// NORTH EAST
		else if (mv_delta.y > 0)
		{
			T_dir = TRACE_NE;
		}
	}
	else if (mv_delta.x < 0)
	{
		// WEST
		if (mv_delta.y == 0)
		{
			T_dir = TRACE_W;
		}
		// SOUTH WEST
		else if (mv_delta.y < 0)
		{
			T_dir = TRACE_SW;
		}
		// NORTH WEST
		else if (mv_delta.y > 0)
		{
			T_dir = TRACE_NW;
		}
	}
	// SOUTH
	else if (mv_delta.y < 0)
	{
		T_dir = TRACE_S;
	}
	// NORTH
	else if (mv_delta.y > 0)
	{
		T_dir = TRACE_N;
	}
	return T_dir;
}

bool PhysicsComponent::checkTile(CollisionFace* face,
	const Collider& incident_object,
	const Vector2& velocity,
	const Collider& collider)
{

}
