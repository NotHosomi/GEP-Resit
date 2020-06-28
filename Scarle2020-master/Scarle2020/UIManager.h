#pragma once
#include <list>

#include "GameObject2D.h"
#include "UIStates.h"

class UIManager
{
public:
	UIManager() = default;
	~UIManager() = default;

	void updateSceneData(std::list<GameObject2D*> _objects);
	ButtonTypes checkUIClicked(Vector2 _mousePos);
	void adUI(GameObject2D* _ui) { m_activeUI.push_back(_ui); };

private:
	//void executeButton();

	std::list<GameObject2D*> m_activeUI;
	ButtonTypes m_currentClicked;
};

