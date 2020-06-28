#ifndef _GAME_OBJECT_2D_H_
#define _GAME_OBJECT_2D_H_

//=================================================================
//the base Game Object 2D
//=================================================================

#include <d3d11_1.h>
#include "SimpleMath.h"
#include <string>

#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "TextComponent.h"
#include "ButtonComponent.h"
#include "PlayerComponent.h"
#include "ObjectData.h"
#include "WeaponComponent.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace std;

struct GameData;
struct DrawData2D;

class GameObject2D
{
public:
	GameObject2D();
	virtual ~GameObject2D() {};

	//GameObject2D(const GameObject2D& button) {};          // Copy Constructor
	//GameObject2D& operator=(const GameObject2D& button); // Copy Assignment Operator

	void init(BasicObject _obj, ID3D11Device* _GD, ID3D11DeviceContext* _deviceContext);
	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);

	//setters
	void SetPos(Vector2 _pos);
	void SetRot(float _rot) { m_rotation = _rot; }
	void SetColour(Color _colour) { m_colour = _colour; }
	void SetScale(Vector2 _scale) { m_scale = _scale; }
	void SetScale(float _scale) { m_scale = _scale * Vector2::One; }
	void SetOrigin(Vector2 _origin) { m_origin = _origin; }
	//getters
	Vector2 GetPos() { return m_pos; }
	float GetRot() { return m_rotation; }
	Color GetColour() { return m_colour; }
	Vector2 GetScale() { return m_scale ; }
	Vector2 GetOrigin() { return m_origin ; }
	float getWidth() { return m_width; }
	float getHeight() { return m_height; }
	void setWidth(int _value) { m_width = _value; }
	void setheight(int _value) { m_height = _value; }

	//components
	void SetSprite(std::string _fileName, ID3D11Device* _GD);
	SpriteComponent* spriteComp() { return m_spriteComp; }
	void copySpriteComp(SpriteComponent* _comp);

	void AddPhysicsComponent(Vector2 _startPos);
	PhysicsComponent* physicsComponent() { return m_physicsComp; }
	void copyPhysicsComp(PhysicsComponent* _comp);
	
	void AddCollisionComponent();
	CollisionComponent* collisionComponent(){ return m_collisionComp;}
	void copyCollisionComp(CollisionComponent* _comp);
	
	void AddButtonComponent(ButtonTypes _type);
	ButtonComponent* buttonComponent() { return m_buttonComp; }

	void AddTextComponent();
	TextComponent* textComponent() { return m_textComponent; }

	void AddPlayerComponent();
	PlayerComponent* playerComponent() { return m_playerComp; };

	void AddWeaponComponent(WeaponData::AttackTypes _attack, WeaponData::AimTypes _aim);
	WeaponComponent* weaponComponent() { return m_weaponComp; }
	void copyWeaponComp(WeaponComponent* _comp);

protected:
	Vector2 m_pos;
	float m_rotation;
	Vector2 m_scale;
	Color m_colour;
	Vector2 m_origin;

	int m_width = 0;
	int m_height = 0;

	PhysicsComponent* m_physicsComp = nullptr;
	SpriteComponent* m_spriteComp = nullptr;
	CollisionComponent* m_collisionComp = nullptr;
	ButtonComponent* m_buttonComp = nullptr;
	TextComponent* m_textComponent = nullptr;
	PlayerComponent* m_playerComp = nullptr;
	WeaponComponent* m_weaponComp = nullptr;

	//move somewhere else e.g. weapon comp
	
	bool isHoming = false;
};
#endif