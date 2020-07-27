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
		IN_FIRE = 16,
		IMP_JUMP = 32,
		IMP_NEXT = 64,
		IMP_PREV = 128
	};
	void updateInputs(const DirectX::Keyboard::KeyboardStateTracker* keys);
	void releaseKey(Inputs key);
	void pressKey(Inputs key);
	bool checkKey(Inputs key);
	bool any();
private:
	int m_keylist = 0;
};

