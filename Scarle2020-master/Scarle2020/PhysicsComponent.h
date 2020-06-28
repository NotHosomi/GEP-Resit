#pragma once
#include <d3d11_1.h>
#include <stack>
#include <queue>

#include <d3d11_1.h>
#include "SimpleMath.h"
#include <string>
#include "PhysicsData.h"

using namespace DirectX::SimpleMath;

class PhysicsComponent
{

public:
	PhysicsComponent(Vector2 _startPos);
	~PhysicsComponent() = default;

	void setXVector(const float& x_);
	void setYVector(const float& y_);
	void setVelocity(const float& x_, const float& y_);
	void setVelocity(const Vector2& vel_);

	void setXSpeed(const float speed_);
	void setYSpeed(const float& speed_);
	void setSpeed(Vector2 speed);


	Vector2 getVelocity() const;
	float getXVector() const;
	float getYVector() const;

	float getXSpeed() const;
	float getYSpeed() const;

	void invertVelocity(bool x, bool y);

	void normaliseVelocity();

	void move(float _deltaTime, Vector2& pos);
	void move(float _deltaTime, Vector2& _pos, Vector2 _velocity);
	void moveToTarget(float _deltaTime, Vector2& pos, Vector2& _targetPos);
	void jump(float _deltaTime, float y);
	void rotate(float _rotation, float _pi, float _deltaTime);
	void bounce(float _deltaTime, Vector2& _pos, bool _reflectX, bool _reflectY);


	void applyGravity(float _yPos, float _force, float _deltaTime);
	float getJumpedPos() { return m_jumpPos; }

	Vector2 getPos() { return m_pos; }
	void setPos(Vector2 _pos) { m_pos = _pos; }
	Vector2 repulseForce(Vector2 _objPos, Vector2 _pixelPos);

	PhysicsStates getCurrentState();
	void addState(PhysicsStates _state);

	float getRotation() { return m_rotation; }

	void setGravity(bool _value) { m_gravityApplies = _value; }
	bool getGravity() { return m_gravityApplies; }

	void setBounce(bool _value) { m_isBouncie = _value; }
	bool getBounce() { return m_isBouncie; }

	void setIsRotating(bool _value) { m_isRotating = _value; }
	bool getIsRotating() { return m_isRotating; }

	void setIsGrounded(bool _value) { m_grounded = _value; }
	bool getIsGrounded() { return m_grounded; }
private:
	Vector2 m_pos = { 0, 0 };
	Vector2 m_velocity = { 0, 0 };
	float m_xSpeed = 0.0f;
	float m_ySpeed = 0.0f;
	float m_jumpPos = 0.0f;
	float m_rotation = 0.0f;
	bool m_isBouncie = false;
	bool m_gravityApplies = true;
	bool m_isRotating = true;
	bool m_isStatic = false;
	float m_friction = 1.01f;
	bool m_grounded = false;
	//have a stack of states/vector
	std::stack<PhysicsStates> m_states;
};

