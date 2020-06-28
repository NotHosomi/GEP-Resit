#pragma once
#include "ImageGO2D.h"

class DestructionCircle : public GameObject2D
{
public:
	DestructionCircle(string name, ID3D11Device* _GD, float raduis, Vector2 pos, std::list<GameObject2D*> players, GameObject2D* _currentWeapon);
protected:
	string m_fileName = "";

};

