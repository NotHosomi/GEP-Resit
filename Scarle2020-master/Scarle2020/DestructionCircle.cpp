#include "pch.h"
#include "DestructionCircle.h"
#include <list>

DestructionCircle::DestructionCircle(string _fileName, ID3D11Device* _GD, float raduis, Vector2 pos, std::list<GameObject2D*> players, GameObject2D* _currentWeapon)
{
	SetScale(raduis);
	Vector2 newPos  ((pos.x - 136 * m_scale.x) , (pos.y - 136 * m_scale.y));
	SetPos(newPos);

	m_spriteComp = new SpriteComponent(_fileName, _GD);
	m_collisionComp = new CollisionComponent();
	m_collisionComp->Init(136 * m_scale.x, pos);

	for (auto& obj : players)
	{
		obj->collisionComponent()->updateRect(obj->GetPos().x, obj->GetPos().y, obj->getWidth(), obj->getHeight());
		if (obj->playerComponent() && m_collisionComp->getCircularCollision(obj->collisionComponent()))
		{
			obj->playerComponent()->damage(_currentWeapon->weaponComponent()->damage());
			obj->textComponent()->text(std::to_string(obj->playerComponent()->health()));
			if (obj->playerComponent()->health() <= 0)
				obj->spriteComp()->setVisibility(false);
		}
	}
 }
