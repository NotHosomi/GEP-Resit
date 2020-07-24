#pragma once
#include "GameObject2D.h"
#include <list>

#include "DestructionCircle.h"

class PhysicsManager
{
public:
	PhysicsManager() = default;
	
	void UpdatePhysicsEngine(RenderTarget* _renderTarget, ID3D11DeviceContext* _d3dContext, GameData* _GD, list<GameObject2D*>& objects, ID3D11Device* _device,
							list<GameObject2D*>& _alphaCircle, GameObject2D* _missile, GameObject2D* _currentPlayer);
	void MaintainScene(list<GameObject2D*> objects, GameData* _GD);
	void MaintainCollisions(list<GameObject2D*>& objects, list<GameObject2D*> colliders, GameData* _GD,
		ID3D11Device* _device, list<GameObject2D*>& _alphaCircle, GameObject2D* _currentWorm);
	void terrainCollision(RenderTarget* _renderTarget, ID3D11DeviceContext* _d3dContext, GameData* _GD, list<GameObject2D*>& objects, ID3D11Device* _device, list<GameObject2D*>& _alphaCircle, GameObject2D* _missile);

	Vector2 m_up = Vector2(0, -0.1);
	Vector2 m_down = Vector2(0, 0.01);
	Vector2 m_right = Vector2(0.0f, -0.01f);
	Vector2 m_left = Vector2(0.0f, -0.01f);
private:

	bool test = false;

	DestructionCircle* m_DC = nullptr;
	GameObject2D* remove = nullptr;
};

