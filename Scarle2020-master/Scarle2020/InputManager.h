#pragma once
#include "Keyboard.h"
class InputManager
{
public:
	enum Inputs
	{
		IN_LEFT = 1,
		IN_RIGHT = 2,
		IN_UP = 4,
		IN_DOWN = 8,
		IN_JUMP = 16,
		IN_FIRE = 32,
		IN_NEXT = 64,
		IN_PREV = 128
	};
	void updateInputs(const DirectX::Keyboard::KeyboardStateTracker* keys);
	void releaseKey(Inputs key);
	void pressKey(Inputs key);
	bool checkKey(Inputs key);
private:
	int m_keylist = 0;
};

