#include "pch.h"
#include "InputManager.h"


void InputManager::updateInputs(const DirectX::Keyboard::KeyboardStateTracker* keys)
{
	// All keybinds defined in one place! How convenient
	// Although, it'd be nicer if this input updating was happening on a separate thread honestly
	if (keys->pressed.A || keys->pressed.J)
	{
		m_keylist |= IN_LEFT;
	};
	if (keys->pressed.D || keys->pressed.K)
	{
		m_keylist |= IN_RIGHT;
	}
	if (keys->pressed.W)
	{
		m_keylist |= IN_UP;
	}
	if (keys->pressed.S)
	{
		m_keylist |= IN_DOWN;
	}
	if (keys->pressed.Space || keys->pressed.I)
	{
		m_keylist |= IN_JUMP;
	}
	if (keys->pressed.F)
	{
		m_keylist |= IN_FIRE;
	}
	if (keys->pressed.E)
	{
		m_keylist |= IN_NEXT;
	}
	if (keys->pressed.Q)
	{
		m_keylist |= IN_PREV;
	}
}

void InputManager::releaseKey(Inputs key)
{
	m_keylist &= ~key;
}

void InputManager::pressKey(Inputs key)
{
	m_keylist |= key;
}

bool InputManager::checkKey(Inputs key)
{
	return static_cast<bool>(m_keylist & key);
}

bool InputManager::any()
{
	return static_cast<bool>(m_keylist);
}
