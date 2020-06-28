#include "pch.h"
#include "PhysicsComponent.h"
#include <math.h>


PhysicsComponent::PhysicsComponent(Vector2 _startPos)
{
	m_states.push(PhysicsStates::Idle);
	m_pos = _startPos;
}

void PhysicsComponent::setXVector(const float& x_)
{
	m_velocity.x = x_;
}

void PhysicsComponent::setYVector(const float& y_)
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

void PhysicsComponent::setXSpeed(const float speed_)
{
	m_xSpeed = speed_;
}

void PhysicsComponent::setYSpeed(const float& speed_)
{
	m_ySpeed = speed_;
}

Vector2 PhysicsComponent::getVelocity() const
{
	return m_velocity;
}

float PhysicsComponent::getXVector() const
{
	return m_velocity.x;
}

float PhysicsComponent::getYVector() const
{
	return m_velocity.y;
}

float PhysicsComponent::getXSpeed() const
{
	return m_xSpeed;
}

float PhysicsComponent::getYSpeed() const
{
	return m_ySpeed;
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
	normaliseVelocity();

	float x = pos.x;
	float y = pos.y;

	x += m_velocity.x * m_xSpeed * _deltaTime;
	y += m_velocity.y * m_ySpeed * _deltaTime;
	
	m_pos.x = x;
	m_pos.y = y;

	m_states.push(PhysicsStates::UpdatePositonData);
}

void PhysicsComponent::move(float _deltaTime, Vector2& _pos, Vector2 _velocity)
{
	float x = _pos.x;
	float y = _pos.y;

	x += _velocity.x * m_xSpeed * _deltaTime;
	y += _velocity.y * m_ySpeed * _deltaTime;

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

void PhysicsComponent::jump(float _deltaTime, float y)
{
	m_jumpPos = y - m_ySpeed * _deltaTime;
	m_pos.y = m_jumpPos;
	m_states.push(PhysicsStates::Jumped);
}

void PhysicsComponent::rotate(float _rotation, float _pi, float _deltaTime)
{
	if (!m_isRotating)
		return;

	float y_vect = getYVector() + 1 * _deltaTime;
	setYVector(y_vect);
	float angle = atan(getYVector() / getXVector());

	if (getXVector() < 0)
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
	
	m_xSpeed /= m_friction;
	if(m_velocity.y > -0.6 && m_velocity.y < 0)
		m_velocity.y = -0.6;
	
	move(_deltaTime, _pos);
}


void PhysicsComponent::applyGravity(float _yPos, float _force, float _deltaTime)
{
	if (!m_gravityApplies)
		return;
	float y = _yPos + _force * _deltaTime;
	m_pos.y = y;
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



