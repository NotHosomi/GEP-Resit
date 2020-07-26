#include "pch.h"
#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(Vector2 _dimensions, float _weight)
{
	self.width = _dimensions.x;
	self.height = _dimensions.y;
	weight = _weight;
}

void PhysicsComponent::move(float dt, Grid* world, Vector2& pos)
{
	if (grounded)
	{
		applyFriction(dt);
	}
	else
	{
		velocity.y += weight * dt;
	}


	Vector2 frame_velocity = velocity;
	frame_velocity *= dt;
	frame_velocity = checkCollisions(world, self, frame_velocity);
	pos += frame_velocity;
	// update collider position
	self.x = pos.x;
	self.y = pos.y;

	// update persistent velocity to reflect the changes in checkCollisions();
	velocity = frame_velocity / dt;
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

// fast collision check, used to objects that die on contact
bool PhysicsComponent::checkCollisionsCheap(Grid* world, const Collider& object, const Vector2& mv_delta)
{
	vector<Tile*> tile_list = genTileList(world, object, mv_delta);
	return std::any_of(tile_list.begin(), tile_list.end(), [](const Tile* tile)
	{
		if (tile == nullptr)
		{
			return false;
		}
		return tile->isAlive();
	});
}

// Comprehensive collision check, used for persistent objects
Vector2 PhysicsComponent::checkCollisions(Grid* world, const Collider& object, const Vector2& mv_delta)
{
	Vector2 output = mv_delta;
	vector<Vector2> orig_list = genOrigList(object);
	vector<Vector2> dest_list = genDestList(orig_list, mv_delta);
	vector<Tile*> tile_list = genTileList(world, dest_list);
	TraceDir dir = genTraceDir(mv_delta);

#if 1 // DEBUG
	bool potential = std::any_of(tile_list.begin(), tile_list.end(), [](const Tile* tile)
		{
			if (tile == nullptr)
			{
				return false;
			}
			return tile->isAlive();
		});
	if (potential)
	{
		int x = 1;
	}
#endif

	switch (dir)
	{
	case TRACE_N:
		if (checkTile(tile_list[0]) || checkTile(tile_list[3]))
			if (output.y < 0)
			{
				output.y = 0;
			}
		break;
	case TRACE_S:
		if (checkTile(tile_list[2]) || checkTile(tile_list[5]))
			if (output.y > 0)
			{
				grounded = true;
				if (bouncy)
				{
					output.y *= -0.5;
				}
				else
				{
					output.y = 0;
				}
			}
		break;
	case TRACE_W:
		if (checkTile(tile_list[0]) || checkTile(tile_list[1]) || checkTile(tile_list[2]))
			if (output.x > 0)
			{
				if (grounded)
					output.x = 0;
				else
					output.x *= -0.5;
			}
		break;
	case TRACE_E:
		if (checkTile(tile_list[3]) || checkTile(tile_list[4]) || checkTile(tile_list[5]))
			if (output.x > 0)
			{
				if (grounded)
					output.x = 0;
				else
					output.x *= -0.5;
			}
		break;
	default:
		complexTrace(orig_list, output, tile_list, dir);
		break;
	}
	return output;
}

vector<Vector2> PhysicsComponent::genOrigList(const Collider& object)
{
	vector<Vector2> orig_list;
	Vector2 checkCoord;
	// Upper Left
	checkCoord.x = object.x;
	checkCoord.y = object.y;
	orig_list.emplace_back(checkCoord);
	// Middle Left
	checkCoord.x = object.x;
	checkCoord.y = object.y + object.height / 2;
	orig_list.emplace_back(checkCoord);
	// Bottom Left
	checkCoord.x = object.x;
	checkCoord.y = object.y + object.height;
	orig_list.emplace_back(checkCoord);
	// Upper Right
	checkCoord.x = object.x + object.width;
	checkCoord.y = object.y;
	orig_list.emplace_back(checkCoord);
	// Middle Right
	checkCoord.x = object.x + object.width;
	checkCoord.y = object.y + object.height / 2;
	orig_list.emplace_back(checkCoord);
	// Bottom Right
	checkCoord.x = object.x + object.width;
	checkCoord.y = object.y + object.height;
	orig_list.emplace_back(checkCoord);
	return orig_list;
}

vector<Vector2> PhysicsComponent::genDestList(const vector<Vector2>& orig_list, const Vector2& mv_delta)
{
	vector<Vector2> dest_list;
	for (auto& coord : orig_list)
	{
		dest_list.emplace_back(coord + mv_delta);
	}
	return dest_list;
}

vector<Tile*> PhysicsComponent::genTileList(Grid* world, const vector<Vector2>& dest_list)
{
	vector<Tile*> tile_list;
	for (auto& coord : dest_list)
	{
		tile_list.emplace_back(world->findTile(coord));
	}
	return tile_list;
}

vector<Tile*> PhysicsComponent::genTileList(Grid* world, const Collider& object, const Vector2& mv_delta)
{
	vector<Tile*> tile_list;
	Vector2 checkCoord;
	// Upper Left 
	checkCoord.x = object.x + mv_delta.x;
	checkCoord.y = object.y + mv_delta.y;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Middle Left 
	checkCoord.x = object.x + mv_delta.x;
	checkCoord.y = object.y + mv_delta.y + object.height / 2;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Bottom Left 
	checkCoord.x = object.x + mv_delta.x;
	checkCoord.y = object.y + mv_delta.y + object.height;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Upper Right 
	checkCoord.x = object.x + mv_delta.x;
	checkCoord.y = object.y + mv_delta.y;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Middle Right 
	checkCoord.x = object.x + mv_delta.x;
	checkCoord.y = object.y + mv_delta.y + object.height / 2;
	tile_list.emplace_back(world->findTile(checkCoord));
	// Bottom Right 
	checkCoord.x = object.x + mv_delta.x;
	checkCoord.y = object.y + mv_delta.y + object.height;
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
		else if (mv_delta.y > 0)
		{
			T_dir = TRACE_SE;
		}
		// NORTH EAST
		else if (mv_delta.y < 0)
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
		else if (mv_delta.y > 0)
		{
			T_dir = TRACE_SW;
		}
		// NORTH WEST
		else if (mv_delta.y < 0)
		{
			T_dir = TRACE_NW;
		}
	}
	// SOUTH
	else if (mv_delta.y > 0)
	{
		T_dir = TRACE_S;
	}
	// NORTH
	else if (mv_delta.y < 0)
	{
		T_dir = TRACE_N;
	}
	return T_dir;
}

void PhysicsComponent::complexTrace(vector<Vector2> orig_list, Vector2& mv_delta,
	vector<Tile*> tile_list, PhysicsComponent::TraceDir dir)
{
	for (int i = 0; i < orig_list.size(); ++i)
	{
		if (tile_list[i]->isAlive())
		{
			Collider tile_col = tile_list[i]->generateCollider();
			vertexProject(mv_delta, orig_list[i], tile_col, dir);
		}
	}
}

bool PhysicsComponent::vertexProject(Vector2 &mv_delta, Vector2 origin,
	Collider otherHitbox, PhysicsComponent::TraceDir dir)
{
	Vector2 targetCoord = Vector2(otherHitbox.x, otherHitbox.y);
	int face = 0;
	bool vertical_collision;
	switch (dir)
	{
	case TRACE_NE:
		targetCoord.y += otherHitbox.height;
		vertical_collision = !isCoordUnderVector(origin, mv_delta, targetCoord);
		break;
	case TRACE_SE:
		vertical_collision = isCoordUnderVector(origin, mv_delta, targetCoord);
		break;
	case TRACE_SW:
		targetCoord.x += otherHitbox.width;
		vertical_collision = isCoordUnderVector(origin, mv_delta, targetCoord);
		break;
	case TRACE_NW:
		targetCoord.x += otherHitbox.width;
		targetCoord.y += otherHitbox.height;
		vertical_collision = !isCoordUnderVector(origin, mv_delta, targetCoord);
		break;
	}

	float numerator;
	float denominator;
	if (vertical_collision)
	{
		numerator = targetCoord.y - origin.y;
		denominator = mv_delta.y;
	}
	else
	{
		numerator = targetCoord.x - origin.x;
		denominator = mv_delta.x;
	}
	numerator = abs(numerator);
	denominator = abs(denominator);
	if (numerator > denominator)
	{
		return false;
	}
	// numerator -= 0.01;

	float truncation = numerator / denominator;
	const Vector2 in_vel = mv_delta;
	mv_delta *= truncation;

	if (mv_delta.Length() >= in_vel.Length())
	{
		mv_delta = in_vel;
		return false;
	}
	return true;
}

bool PhysicsComponent::isCoordUnderVector(const Vector2& origin,
	const Vector2& delta, Vector2 coord)
{
	// move to 0, 0
	coord = coord - origin;
	return (coord.y > delta.y * (coord.x / delta.x));
}

bool PhysicsComponent::checkTile(const Tile* tile) const
{
	if (tile == nullptr)
	{
		return false;
	}
	return tile->isAlive();
}
