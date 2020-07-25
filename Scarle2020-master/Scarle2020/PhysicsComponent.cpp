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
	float speed = velocity.Length;
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
