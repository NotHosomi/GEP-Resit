#include "pch.h"
#include "PhysicsComponent.h"
#include <math.h>


PhysicsComponent::PhysicsComponent(Vector2 _startPos)
{
	m_states.push(PhysicsStates::Idle);
	m_pos = _startPos;
}

void PhysicsComponent::setVelX(const float& x_)
{
	m_velocity.x = x_;
}

void PhysicsComponent::setVelY(const float& y_)
{
	m_velocity.y = y_;
}

void PhysicsComponent::setVelocity(const float& x_, const float& y_)
{
	m_velocity.x = x_;
	m_velocity.y = y_;
}

void PhysicsComponent::setVelocity(const Vector2& vel_)
{
	m_velocity = vel_;
}

Vector2 PhysicsComponent::getVelocity() const
{
	return m_velocity;
}

float PhysicsComponent::getVelX() const
{
	return m_velocity.x;
}

float PhysicsComponent::getVelY() const
{
	return m_velocity.y;
}

void PhysicsComponent::invertVelocity(bool x, bool y)
{
	if(x)
		m_velocity.x *= -1;

	if (y)
		m_velocity.y *= -1;
}

void PhysicsComponent::normaliseVelocity()
{
	//SIMON:: m_velocity.Normalize();

	float magnitude = sqrtf((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));
	if (magnitude)
	{
		m_velocity.x /= magnitude;
		m_velocity.y /= magnitude;
	}
}

void PhysicsComponent::move(float _deltaTime, Vector2& pos)
{
	if (m_grounded)
	{
		setVelY(-0.1); // setting to 0 makes the worms disappear, because ?????
		applyFriction(_deltaTime);
	}
	applyGravity(_deltaTime);

	// Truncate velocity if travelling to fast
	if (m_velocity.Length() > MV_MAXSPEED)
	{
		float drop = MV_MAXSPEED / m_velocity.Length();
		m_velocity *= drop;
	}


	float x = pos.x;
	float y = pos.y;

	x += m_velocity.x * _deltaTime * MOVE_MODI;
	y += m_velocity.y * _deltaTime * MOVE_MODI;
	
	m_pos.x = x;
	m_pos.y = y;

	//m_states.push(PhysicsStates::UpdatePositonData);
}

void PhysicsComponent::nudge(float _deltaTime, Vector2& _pos, Vector2 _velocity)
{
	float x = _pos.x;
	float y = _pos.y;
	
	x += _velocity.x * _deltaTime * MOVE_MODI;
	y += _velocity.y * _deltaTime * MOVE_MODI;
	
	m_pos.x = x;
	m_pos.y = y;
	m_states.push(PhysicsStates::UpdatePositonData);
}

void PhysicsComponent::moveToTarget(float _deltaTime, Vector2& pos, Vector2& _targetPos)
{
	Vector2 direction = _targetPos - pos;
	m_velocity = direction;
	move(_deltaTime, pos);
}

void PhysicsComponent::jump(float _deltaTime)
{
	if (m_grounded)
	{
		m_velocity.y = JUMP_FORCE / m_weight;
		m_grounded = false;
		m_states.push(PhysicsStates::Jumped);
	}
}

void PhysicsComponent::rotate(float _rotation, float _pi, float _deltaTime)
{
	if (!m_isRotating)
		return;

	float y_vect = getVelY() + 1 * _deltaTime;
	setVelY(y_vect);
	float angle = atan(getVelY() / getVelX());

	if (getVelX() < 0)
	{
		angle += (_pi);
	}

	m_rotation = angle;

	m_states.push(PhysicsStates::UpdateRotationData);
}

void PhysicsComponent::bounce(float _deltaTime, Vector2& _pos, bool _reflectX, bool _reflectY)
{
	if (_reflectX)
		m_velocity.x *= -1;
	if (_reflectY)
		m_velocity.y *= -1;
	
	m_velocity.x /= m_friction;
	if(m_velocity.y > -0.6 && m_velocity.y < 0)
		m_velocity.y = -0.6;
	
	move(_deltaTime, _pos);
}


void PhysicsComponent::applyGravity(float _deltaTime)
{
	//if (m_gravityApplies && !m_grounded)
	if (!m_grounded)
	{
		m_velocity.y += m_weight * _deltaTime;
	}
	else
	{
		// m_velocity.y = 0;
	}
}

/*
find the direction vector from the pixel collided and the centre of the worm then apply force there but use trig to calculate 
amount of force or apply force to appose moving vector
*/
Vector2 PhysicsComponent::repulseForce(Vector2 _objPos, Vector2 _pixelPos)
{
	//calculate vector needed
	Vector2 direction = -_objPos + _pixelPos;
	return direction;
}

PhysicsStates PhysicsComponent::getCurrentState()
{
	PhysicsStates state = m_states.top();
	if (state != PhysicsStates::Idle)
	{
		m_states.pop();
	}
	return state;
}

void PhysicsComponent::addState(PhysicsStates _state)
{
	m_states.push(_state);
}

void PhysicsComponent::playerMove(InputManager* inputs, float dt)
{
	if (!m_grounded)
	{
		return;
	}
	if (inputs->checkKey(InputManager::IN_JUMP))
	{
		m_velocity.x *= 2;
		m_velocity.y = JUMP_FORCE;
		m_grounded = false;
	}
	if (inputs->checkKey(InputManager::IN_LEFT) != inputs->checkKey(InputManager::IN_RIGHT))
	{
		if (inputs->checkKey(InputManager::IN_LEFT))
		{
			m_velocity.x -= MV_ACCELERATION / dt;
		}
		else
		{
			m_velocity.x += MV_ACCELERATION / dt;
		}
	}
}

void PhysicsComponent::applyFriction(float dt)
{
	float speed = m_velocity.Length();
	//if (speed = 0)
	//{
	//	return;
	//}
	float drop = speed * MV_FRICTION * dt;
	float newspeed = speed - drop;
	if (newspeed < 0)
		newspeed = 0;
	newspeed /= speed;
	m_velocity *= newspeed;
}



