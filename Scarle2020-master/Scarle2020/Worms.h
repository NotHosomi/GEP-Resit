#pragma once
#include "GameObject2D.h"

class Worms : public GameObject2D
{
public:
	Worms() = default;
	~Worms() = default;

	void Init(string _fileName, ID3D11Device* _GD);

	void Tick(GameData* _GD)override;

	void setHealth(float _health);
	float getHealth();

	//bool loadAssets();
	//void onUpdate();
	//void onRender();

private:
	const float m_SCALE = 0.05f;
	const Vector2 m_spawnPos = Vector2(10, 190);
	const float speed = 5.0f;
	float player_health = 100.0f;

	//varibles 
	bool grounded;
};

