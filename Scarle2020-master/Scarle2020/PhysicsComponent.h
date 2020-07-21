#pragma once
#include <d3d11_1.h>
#include <stack>
#include <queue>

#include <d3d11_1.h>
#include "SimpleMath.h"
#include <string>
#include "PhysicsData.h"
#include "InputManager.h"

using namespace DirectX::SimpleMath;

class PhysicsComponent
{

public:
	PhysicsComponent(Vector2 _startPos);
	~PhysicsComponent() = default;

	void setVelX(const float& x_);
	void setVelY(const float& y_);
	void setVelocity(const float& x_, const float& y_);
	void setVelocity(const Vector2& vel_);

	Vector2 getVelocity() const;
	float getVelX() const;
	float getVelY() const;

	void invertVelocity(bool x, bool y);

	void normaliseVelocity();

	void move(float _deltaTime, Vector2& pos);
	void move(float _deltaTime, Vector2& _pos, Vector2 _velocity);
	void moveToTarget(float _deltaTime, Vector2& pos, Vector2& _targetPos);
	void jump(float _deltaTime);
	void rotate(float _rotation, float _pi, float _deltaTime);
	void bounce(float _deltaTime, Vector2& _pos, bool _reflectX, bool _reflectY);


	void applyGravity(float _deltaTime);

	Vector2 getPos() { return m_pos; }
	void setPos(Vector2 _pos) { m_pos = _pos; }
	Vector2 repulseForce(Vector2 _objPos, Vector2 _pixelPos);

	PhysicsStates getCurrentState();
	void addState(PhysicsStates _state);

	float getRotation() { return m_rotation; }

	void setGravity(bool _value) { m_gravityApplies = _value; }
	bool getGravity() { return m_gravityApplies; }

	void setWeight(float _value) { m_weight = _value; }
	float getWeight() { return m_weight; }

	void setBounce(bool _value) { m_isBouncie = _value; }
	bool getBounce() { return m_isBouncie; }

	void setIsRotating(bool _value) { m_isRotating = _value; }
	bool getIsRotating() { return m_isRotating; }

	void setIsGrounded(bool _value) { m_grounded = _value; }
	bool getIsGrounded() { return m_grounded; }

	void playerMove(InputManager* inputs, float dt);
private:
	const float MV_ACCELERATION = 10;
	const float MOVE_MODI = 50;
	const float JUMP_FORCE = 10;
	Vector2 m_pos = { 0, 0 };
	Vector2 m_velocity = { 0, 0 };
	float m_jumpPos = 0.0f;
	float m_rotation = 0.0f;
	bool m_isBouncie = false;
	bool m_gravityApplies = true;
	float m_weight;
	bool m_isRotating = true;
	bool m_isStatic = false;
	float m_friction = 1.01f;
	bool m_grounded = false;
	float m_terminalVel = 60;
	//have a stack of states/vector
	std::stack<PhysicsStates> m_states;
};

