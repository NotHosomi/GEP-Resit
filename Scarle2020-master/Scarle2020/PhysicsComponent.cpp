#include "pch.h"
#include "PhysicsComponent.h"
#include "GameData.h"

PhysicsComponent::PhysicsComponent(Vector2 _dimensions, float _weight, float _elasticity)
{
	self.width = _dimensions.x;
	self.height = _dimensions.y;
	weight = _weight;
	elasticity = _elasticity;
}

// Returns true if there was a collision
bool PhysicsComponent::move(float dt, Grid* world, Vector2& pos)
{
	if (locked)
	{
		return true;
	}
	bool contact = false;
	checkGrounded(world, self);
	if (grounded)
	{
		if (velocity.y > 0)
		{
			velocity.y *= -elasticity;
			contact = true;
		}
		applyFriction(dt);
	}
	else
	{
		velocity.y += weight * dt;
	}

	Vector2 frame_velocity = velocity;
	frame_velocity *= dt;
	Vector2 old_velo = frame_velocity;
	bool hit = checkCollisions(world, self, frame_velocity);
	// Using the accurate displacement would be cool, however it results in objects actually
	// touching, which then allows them to phase into eachother. Something for the devlog I guess
#if 0
	pos += displacement;
#else
	pos += frame_velocity;
#endif
	moveSelf(pos);
	// update persistent velocity to reflect the changes from checkCollisions();
	velocity = frame_velocity / dt;
	// return true if there was a collision
	return hit || contact;
}

// Update the position of our collider
void PhysicsComponent::moveSelf(const Vector2& new_pos)
{
	float nudge = 0;
	// due to how the collider origin is it's top left corner, but GameObject's pos is their center
	// it's faster to do this collider nudge than to translate between GO origin and Collider origin each frame
	if (self.width > self.height)
	{
		nudge = self.height / 4;
	}
	self.x = new_pos.x - nudge;

	if (self.width < self.height)
	{
		nudge = self.width / 4;
	}
	self.y = new_pos.y - nudge;
}

void PhysicsComponent::checkGrounded(Grid* world, const Collider& object)
{
	grounded = false;
	Vector2 checkCoord;
	Tile* tile;
	// Bottom Left
	checkCoord.x = object.x;
	checkCoord.y = object.y + object.height;
	checkCoord.y += MV_SKIN;
	tile = world->findTile(checkCoord);
	if (checkTile(tile))
	{
		grounded = true;
	}
	// Intermediaries
	if (object.width > Tile::TILE_DIMS)
	{
		// TODO: run checks for objects wider than a tile
	}
	// Bottom Right
	checkCoord.x = object.x + object.width;
	checkCoord.y = object.y + object.height;
	checkCoord.y += MV_SKIN;
	tile = world->findTile(checkCoord);
	if (checkTile(tile))
	{
		grounded = true;
	}
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

#pragma region COLLISION

// fast collision check, used to objects that die on contact
bool PhysicsComponent::checkCollisionsCheap(Grid* world, const Collider& object, const Vector2& mv_delta)
{
	vector<Tile*> tile_list = genTileList(world, object, mv_delta);
	return std::any_of(tile_list.begin(), tile_list.end(),
		[this](const Tile* tile) { return checkTile(tile); });
}

/* Comprehensive collision check, used for persistent objects
*	Returns true if there was a collision
*	mv_delta will be set to how much the player's new velocity is (is modified by delta time, requires reversion)
*/
bool PhysicsComponent::checkCollisions(Grid* world, const Collider& object, Vector2& mv_delta)
{
	vector<Vector2> orig_list = genOrigList(object);
	vector<Vector2> dest_list = genDestList(orig_list, mv_delta);
	vector<Tile*> tile_list = genTileList(world, dest_list);
	PhysicsComponent::TraceDir dir = genTraceDir(mv_delta);

	bool potential = std::any_of(tile_list.begin(), tile_list.end(),
		[this](const Tile* tile) { return checkTile(tile); });
	if (!potential)
	{ return false; }

	switch (dir)
	{
	case TRACE_N:
		if (checkTile(tile_list[0]) || checkTile(tile_list[3]))
			if (mv_delta.y < 0)
			{
				mv_delta.y = 0;
			}
		break;
	case TRACE_S:
		if (checkTile(tile_list[2]) || checkTile(tile_list[5]))
			if (mv_delta.y > 0)
			{
				grounded = true;
				if (bouncy)
				{
					mv_delta.y *= -0.5;
				}
				else
				{
					mv_delta.y = 0;
				}
			}
		break;
	case TRACE_W:
		if (checkTile(tile_list[0]) || checkTile(tile_list[1]) || checkTile(tile_list[2]))
			if (mv_delta.x < 0)
			{
				if (grounded)
					mv_delta.x = 0;
				else
					mv_delta.x *= -elasticity;
			}
		break;
	case TRACE_E:
		if (checkTile(tile_list[3]) || checkTile(tile_list[4]) || checkTile(tile_list[5]))
			if (mv_delta.x > 0)
			{
				if (grounded)
					mv_delta.x = 0;
				else
					mv_delta.x *= -elasticity;
			}
		break;
	default:
		complexTrace(orig_list, mv_delta, tile_list, dir);
		// ended up not using the precise return value from complexTrace, and just
		// using it's mv_delta output instead, for consistency's sake
		break;
	}
	return true;
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

// returns a precise amount to move the object so that it is aligned with the closest collider
Vector2 PhysicsComponent::complexTrace(vector<Vector2> orig_list, Vector2& mv_delta,
	vector<Tile*> tile_list, PhysicsComponent::TraceDir dir)
{
	vector<float> clip_list;
	vector<CollisionFace> face_list;
	for (int i = 0; i < orig_list.size(); ++i)
	{
		if (checkTile(tile_list[i]))
		{
			Collider tile_col = tile_list[i]->generateCollider();
			CollisionFace face = F_NONE;
			float trunc = vertexProject(mv_delta, orig_list[i], tile_col, dir, face);
			clip_list.emplace_back(trunc);
			face_list.emplace_back(face);
		}
	}
	if (clip_list.size() == 0)
	{
		// No tiles were found
		return mv_delta;
	}

	vector<float>::iterator n = std::min_element(clip_list.begin(), clip_list.end());
	float clip = *n;
	int index = std::distance(clip_list.begin(), n);
	CollisionFace closest_face = *(face_list.begin() + index);
	
#pragma region sanity checks

	if (clip > 1)
	{
		// something has gone wrong, just act like nothing happened!
		return mv_delta;
	}
	if (clip < 0)
	{
		clip = 0;
	}
#pragma endregion

	Vector2 output = mv_delta;
	switch (closest_face)
	{
	case F_TOP:
		grounded = true;
	case F_BOTTOM:
		mv_delta.y = 0;
		output.y *= clip;
		output.y = output.y > 0 ? MV_SKIN : -MV_SKIN;
		break;
	case F_LEFT:
	case F_RIGHT:
		mv_delta.x = grounded ? 0 : mv_delta.x * -elasticity;
		output.x *= clip;
		output.x += output.x > 0 ? MV_SKIN : -MV_SKIN;
		break;
	}
	return output;
}

float PhysicsComponent::vertexProject(const Vector2 &mv_delta, const Vector2& origin,
	Collider otherHitbox, PhysicsComponent::TraceDir dir, CollisionFace& OUT_face)
{
	Vector2 targetCoord = Vector2(otherHitbox.x, otherHitbox.y);
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
		OUT_face = mv_delta.y > 0 ? F_TOP : F_BOTTOM;
	}
	else
	{
		numerator = targetCoord.x - origin.x;
		denominator = mv_delta.x;
		OUT_face = mv_delta.x > 0 ? F_LEFT : F_RIGHT;
	}
	// numerator = abs(numerator);
	// denominator = abs(denominator);
	// if (numerator > denominator)
	// {
	// 	OUT_face = F_NONE;
	// 	return 1;
	// }
	return numerator / denominator;
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

#pragma endregion