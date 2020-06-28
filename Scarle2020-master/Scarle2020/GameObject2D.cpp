#include "pch.h"
#include "GameObject2D.h"
#include "gamedata.h"
#include "DrawData2D.h"
#include "helper.h"

using namespace DirectX;

GameObject2D::GameObject2D()
{
	m_colour = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_pos = Vector2::Zero;
	m_rotation = 0.0f;
	m_scale = Vector2::One;
}
//GameObject2D& GameObject2D::operator=(const GameObject2D& obj)
//{
//	if (&obj != this)
//	{
//		// these are the only componets need to be copied because weapon needs them
//		delete m_spriteComp;
//		this->m_spriteComp = obj.m_spriteComp;
//
//		delete m_physicsComp;
//		this->m_physicsComp = obj.m_physicsComp;
//
//		delete m_collisionComp;
//		this->m_collisionComp = obj.m_collisionComp;
//		
//		delete m_weaponComp;
//		this->m_weaponComp = obj.m_weaponComp;
//
//	}
//
//	return *this;
//}
void GameObject2D::init(BasicObject _obj, ID3D11Device* _GD, ID3D11DeviceContext* _deviceContext)
{
	m_pos = _obj.pos;
	m_width = _obj.dimentions.x;
	m_height = _obj.dimentions.y;
	if (m_width == 0)
		m_width = 1;
	if (m_height == 0)
		m_height = 1;
	SetScale(_obj.scale);
	if (_obj.hasSpriteComp)
	{
		SetSprite(_obj.name, _GD);
		if (_obj.hasAnimationComp)
		{
			m_spriteComp->AddAnimationComponent(_obj.dimentions.x /** m_scale.x*/, _obj.dimentions.y /** m_scale.y*/, _obj.animationCount);
			//m_spriteComp->animationComponent()->setAnimationPoint((ID3D11Texture2D*)m_spriteComp->getTexture(), _deviceContext, _GD);
			m_spriteComp->animationComponent()->updateRect();
		}
	}
	if (_obj.hasCollisionComp)
	{
		AddCollisionComponent();
		m_width *= m_scale.x;
		m_height *= m_scale.y;

		if (_obj.hasAnimationComp)
		{
			RECT rect;
			rect.left = GetPos().x;
			rect.top = GetPos().y;
			rect.right = rect.left + m_spriteComp->animationComponent()->width() * m_scale.x;
			rect.bottom = rect.top + m_spriteComp->animationComponent()->height() * m_scale.y;
			m_collisionComp->Init(rect);
			m_height = m_height / _obj.animationCount;
		}
		else
		{
			RECT rect;
			rect.left = GetPos().x;
			rect.top = GetPos().y;
			rect.right = rect.left + m_width;
			rect.bottom = rect.top + m_height;
			m_collisionComp->Init(rect);
		}
	}
	if (_obj.hasPhysicsComp)
	{
		AddPhysicsComponent(m_pos);
		physicsComponent()->setXSpeed(_obj.speed.x);
		physicsComponent()->setYSpeed(_obj.speed.y);
		physicsComponent()->setBounce(_obj.isBouncie);
		physicsComponent()->setGravity(_obj.useGravity);
	}
	if (_obj.hasButtonComp)
	{
		AddButtonComponent(_obj.buttonType);
	}
	if (_obj.hasTextComp)
	{
		AddTextComponent();
		textComponent()->text(_obj.uiText);
		textComponent()->setColour(_obj.color);
		textComponent()->setScale(_obj.textScale);
		
	}
	if (_obj.hasPlayerComp)
	{
		AddPlayerComponent();
		m_playerComp->health(100);
		m_playerComp->setTeam(_obj.team, _obj.playerID);
		if (textComponent())
		{
			textComponent()->text("100");
			textComponent()->setYOffSet(-20);
			textComponent()->setXOffSet(15);
		}
	}
	if (_obj.hasWeaponComp)
	{
		AddWeaponComponent(_obj.attackType, _obj.aimType);
		m_weaponComp->setExplodeTime(_obj.exlpodeCount);
		m_weaponComp->damage(_obj.damageAmount);
		m_weaponComp->explosionScale(_obj.explosionScale);
		m_weaponComp->ammoCount(_obj.ammoCount);
		m_weaponComp->setRequiresCharge(_obj.needsCharging);
		m_weaponComp->fireRate(_obj.fireRate);
		m_weaponComp->setSpread(_obj.spread);
		if (_obj.weaponSprite != "NULL")
		{
			m_weaponComp->AddSpriteComp(_obj.weaponSprite, _GD);
			m_weaponComp->spriteComp()->setVisibility(false);
			m_weaponComp->setWidth(_obj.weaponWidth);
			m_weaponComp->setHeight(_obj.weaponHeight);
		}
	}


}

//Sets sprite if there isnt already one
void GameObject2D::SetSprite(std::string _fileName, ID3D11Device* _GD)
{
	if (m_spriteComp)
	{
		delete(m_spriteComp);
	}
	m_spriteComp = new SpriteComponent(_fileName, _GD);
}

void GameObject2D::copySpriteComp(SpriteComponent* _comp)
{
	m_spriteComp = new SpriteComponent(*_comp);
}

void GameObject2D::AddPhysicsComponent(Vector2 _startPos)
{
	if (m_physicsComp)
	{
		delete(m_physicsComp);
	}
	m_physicsComp = new PhysicsComponent(_startPos);
}

void GameObject2D::copyPhysicsComp(PhysicsComponent* _comp)
{
	m_physicsComp = new PhysicsComponent(*_comp);
}

void GameObject2D::AddCollisionComponent()
{
	if (m_collisionComp)
	{
		delete(m_collisionComp);
	}
	m_collisionComp = new CollisionComponent();
}

void GameObject2D::copyCollisionComp(CollisionComponent* _comp)
{
	m_collisionComp = new CollisionComponent(*_comp);
}

void GameObject2D::AddButtonComponent(ButtonTypes _type)
{
	if (m_buttonComp)
	{
		delete(m_buttonComp);
	}
	m_buttonComp = new ButtonComponent(_type);
}

void GameObject2D::AddTextComponent()
{
	if (m_textComponent)
	{
		delete(m_textComponent);
	}
	m_textComponent = new TextComponent();
}

void GameObject2D::AddPlayerComponent()
{
	if (m_playerComp)
		delete(m_playerComp);

	m_playerComp = new PlayerComponent();
}

void GameObject2D::AddWeaponComponent(WeaponData::AttackTypes _attack, WeaponData::AimTypes _aim)
{
	if (m_weaponComp)
	{
		delete(m_weaponComp);
	}
	m_weaponComp = new WeaponComponent(_attack, _aim);
}

void GameObject2D::copyWeaponComp(WeaponComponent* _comp)
{
	m_weaponComp = new WeaponComponent(*_comp);
}

void GameObject2D::Draw(DrawData2D* _DD)
{
	//nullptr can be changed to a RECT* to define what area of this image to grab
	//you can also add an extra value at the end to define layer depth
	//right click and "Go to Defintion/Declaration" to see other version of this in DXTK
	if (m_spriteComp)
	{
		if (m_spriteComp->animationComponent())
		{
			_DD->m_Sprites->Draw(m_spriteComp->getTexture(), m_pos, &m_spriteComp->animationComponent()->currentRect(), m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
		}
		else
		{
			_DD->m_Sprites->Draw(m_spriteComp->getTexture(), m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
		}
	}
}

void GameObject2D::SetPos(Vector2 _pos)
{
	m_pos = _pos;
	if (m_physicsComp)
	{
		m_physicsComp->setPos(m_pos);
	}
}

void GameObject2D::Tick(GameData* _GD)
{
	if (weaponComponent())
	{
		weaponComponent()->updateTimer(_GD->m_dt);
	}
}