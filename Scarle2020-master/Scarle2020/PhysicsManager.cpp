#include "pch.h"
#include "PhysicsManager.h"

#include <iostream>

/*
called every frame
*/
void PhysicsManager::UpdatePhysicsEngine(RenderTarget* _renderTarget, ID3D11DeviceContext* _d3dContext, GameData* _GD,
										 list<GameObject2D*>& objects, ID3D11Device* _device, list<GameObject2D*>& _alphaCircle,
										 GameObject2D* _missile, GameObject2D* _currentPlayer)
{
	terrainCollision(_renderTarget, _d3dContext, _GD, objects, _device, _alphaCircle, _missile);
	MaintainCollisions(objects, objects, _GD, _device, _alphaCircle, _currentPlayer);
	MaintainScene(objects, _GD);
}

/*
keeps tabs of any physics objects moving in scene
*/
void PhysicsManager::MaintainScene(list<GameObject2D*> objects, GameData* _GD)
{
	for (auto& obj : objects)
	{
		PhysicsComponent* pComp = obj->physicsComponent();
		if (pComp)
		{
			//do physics
			// if more than one state is added to gamoebject at once it will ignore the seocnd/thirf so neeed to loop through untill hit idl state
			PhysicsStates lastState;
#if 0
			// Hos: This seems absurd!!
			// Why wasn't a simple velocity controlled physics system implemented instead of using an entire event stack???
			// In fact, this current model will result in multiple move() calls happening per frame, resulting in inconsistent movement!
			// HOS-TODO: FIX ALL THIS SHIT
			do
			{
				PhysicsStates currentState = pComp->getCurrentState();
				switch (currentState)
				{
				case PhysicsStates::Idle:
					break;
				case PhysicsStates::Jumped:
					// Hos: This was part of the problem!
					//obj->SetPos(Vector2(obj->GetPos().x, pComp->getJumpedPos()));
					//pComp->setJumped(false);
					break;
				case PhysicsStates::UpdatePositonData:
					obj->SetPos(pComp->getPos());
					//pComp->setUpdatePos(false);
					break;
				case PhysicsStates::UseGravity:
					pComp->applyGravity(_GD->m_dt);
					pComp->move(_GD->m_dt, obj->GetPos());
					break;
				case PhysicsStates::UpdateRotationData:
					obj->SetRot(obj->physicsComponent()->getRotation());
				}
				lastState = currentState;
			} while (lastState != PhysicsStates::Idle);
#endif
			// Resit version
			pComp->move(_GD->m_dt, obj->GetPos());
			obj->SetPos(pComp->getPos());
			// Hos Todo check grounded
		}
	}
}
/*
keeps tabs on collision with worms and weapons
*/
void PhysicsManager::MaintainCollisions(list<GameObject2D*>& objects, list<GameObject2D*> colliders, GameData* _GD,
										ID3D11Device* _device, list<GameObject2D*>& _alphaCircle, GameObject2D* _currentWorm)
{
	std::list<GameObject2D*> toDelete;
	bool removedObject = false;
	for (auto& obj : objects)
	{
		CollisionComponent* cComp = obj->collisionComponent();
		PhysicsComponent* pComp = obj->physicsComponent();
		if (!cComp)
		{
			continue;
		}
		if (obj->spriteComp()->animationComponent())
		{
			cComp->updateRect(obj->GetPos().x, obj->GetPos().y, obj->spriteComp()->animationComponent()->width(), obj->spriteComp()->animationComponent()->height());
		}
		else
		{
			cComp->updateRect(obj->GetPos().x, obj->GetPos().y, obj->getWidth(), obj->getHeight());
		}
		for (auto& col : colliders)
		{
			CollisionComponent* colComp = col->collisionComponent();
			if (!colComp || obj == col || _currentWorm == obj || !col->spriteComp()->getVisibility())
			{
				continue;
			}
			colComp->updateRect(col->GetPos().x, col->GetPos().y, col->getWidth(), col->getHeight());
			bool hasCollided = cComp->getCollison(col->GetPos());
			if (hasCollided && col->weaponComponent())
			{
				//do something
				if (col->physicsComponent()->getBounce())
				{
					col->physicsComponent()->bounce(_GD->m_dt, col->GetPos(), true, true);
					continue;
				}
				//std::cout << "destroy missile";
				Vector2 spawnPos = Vector2(col->GetPos().x + col->getWidth() / 2, col->GetPos().y + col->getHeight());
				m_DC = new DestructionCircle("CircleHitAlpha_1", _device, col->weaponComponent()->explosionScale(), spawnPos, objects, col);
				_alphaCircle.push_back(m_DC);
				if (col->spriteComp())
					col->spriteComp()->setVisibility(false);
				m_DC = nullptr;
				
				toDelete.push_back(obj);
				removedObject = true;
			}
		}
	}
	//clea up memory
	for (auto remove : toDelete)
	{
		objects.remove(remove);
	}
	std::list<GameObject2D*> stuff;
	if (removedObject)
	{
		for (auto& obj : objects)
		{
			stuff.push_back(obj);
		}
		objects.clear();
		for (auto& obj : stuff)
		{
			objects.push_back(obj);
		}
	}
}
/*
keeps tabs on collision with terrain
*/
void PhysicsManager::terrainCollision(RenderTarget* _renderTarget, ID3D11DeviceContext* _d3dContext, GameData* _GD, list<GameObject2D*>& objects,
									  ID3D11Device* _device, list<GameObject2D*>& _alphaCircle, GameObject2D* _missile)
{
	for (auto itr = objects.begin(); itr != objects.end(); itr++)
	{
		if ((*itr)->spriteComp() && (*itr)->spriteComp()->getVisibility() == false)
		{
			continue;
		}
		CollisionComponent* cComp = (*itr)->collisionComponent();
		PhysicsComponent* pComp = (*itr)->physicsComponent();
		if (!cComp || !pComp)
			continue;
		if ((*itr)->spriteComp() && (*itr)->spriteComp()->animationComponent())
		{
			int x = (*itr)->GetPos().x;
			int y = (*itr)->GetPos().y ;
			int width = (*itr)->spriteComp()->animationComponent()->width()* (*itr)->GetScale().x;
			int height = (*itr)->spriteComp()->animationComponent()->height() * (*itr)->GetScale().y;
			cComp->updateRect(x, y, width, height);
		}
		else
		{
			cComp->updateRect((*itr)->GetPos().x, (*itr)->GetPos().y, (*itr)->getWidth(), (*itr)->getHeight());
		}
		if ((*itr)->playerComponent())
			cComp->updatePixelPos(20);
		else
			cComp->updatePixelPos(0);

		bool isCollision[4];
		cComp->terrainCollision(_renderTarget, _d3dContext, _GD, isCollision); // 0: top, 1: right, 2: bottom, 3: left
		if (isCollision[0] && !(*itr)->weaponComponent())
		{
			//only when digging
			//pComp->move(_GD->m_dt, (*itr)->GetPos(), m_down);
		}
		if (isCollision[1] && pComp->getVelX() != 0 && !(*itr)->weaponComponent())
		{
			pComp->move(_GD->m_dt, (*itr)->GetPos(), m_left);
			pComp->move(_GD->m_dt, (*itr)->GetPos(), m_up);
		}
		if (isCollision[3] && pComp->getVelX() != 0 && !(*itr)->weaponComponent())
		{
			pComp->move(_GD->m_dt, (*itr)->GetPos(), m_right);
			pComp->move(_GD->m_dt, (*itr)->GetPos(), m_up);
		}
		if (!isCollision[2])
		{
			pComp->addState(PhysicsStates::UseGravity);
		}
		else if ((*itr)->weaponComponent() == nullptr)
		{
			pComp->setIsGrounded(true);
		}
		else if(m_DC == nullptr)
		{
			if (pComp->getBounce())
			{
				pComp->bounce(_GD->m_dt, (*itr)->GetPos(), true, true);
				continue;
			}
			//std::cout << "destroy missile";
			Vector2 spawnPos = Vector2((*itr)->GetPos().x + (*itr)->getWidth() /2, (*itr)->GetPos().y + (*itr)->getHeight());
			m_DC = new DestructionCircle("CircleHitAlpha_1", _device, (*itr)->weaponComponent()->explosionScale(), spawnPos,  objects, (*itr));
			_alphaCircle.push_back(m_DC);
			if((*itr)->spriteComp())
				(*itr)->spriteComp()->setVisibility(false);
			m_DC = nullptr;
			//delete(obj);
			//objects.erase(itr);
		}
	}
}
