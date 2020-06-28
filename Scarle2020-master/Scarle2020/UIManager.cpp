#include "pch.h"
#include "UIManager.h"


#include <iostream>
/*
called when you load a new level or menu page
*/
void UIManager::updateSceneData(std::list<GameObject2D*> _objects)
{
	m_activeUI.clear();
	for (auto &obj : _objects)
	{
		if (obj->buttonComponent())
		{
			m_activeUI.push_back(obj);
		}
	}
}

ButtonTypes UIManager::checkUIClicked(Vector2 _mousePos)
{
	//std::cout << _mousePos.x << "   " << _mousePos.y << std::endl;
	ButtonTypes type = ButtonTypes::empty;
	for (auto& ui : m_activeUI)
	{
		if (ui->spriteComp() && !ui->spriteComp()->getVisibility())
			continue;
		bool collision = ui->collisionComponent()->getCollison(_mousePos);
		if (collision)
		{
			type = ui->buttonComponent()->getButtonType();
			break;//if using queue to store states dont break here
		}
	}
	return type;
	
	//execute buttons
	//buttons will most likely set states in the game manager
	//executeButton();
}
/*
void UIManager::executeButton()
{
	switch (m_currentClicked)
	{
	case ButtonTypes::quitGame:
		//quit game
		PostQuitMessage(0);
		break;

	case ButtonTypes::quitLevel:
		//back to start menu

		break;

	case ButtonTypes::loadNextScene:
		//load next level
		std::cout << "nextScene";


		break;
	}*/
//}
