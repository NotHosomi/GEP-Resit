#pragma once
#include "GameObject2D.h"
class Explosion :
	public GameObject2D
{
public:
	Explosion(ID3D11Device* _GD, Vector2 location, float radius, float damage);
	void Tick(GameData* _GD) override;
	bool hasExpired() { return age > duration; }

private:
	float duration;
	float age;
	Vector2 m_pos;
	float m_radius;
	float m_damage;

};

