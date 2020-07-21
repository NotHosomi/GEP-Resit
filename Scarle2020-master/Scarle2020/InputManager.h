#pragma once
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
		IN_NEXTWEP = 64,
		IN_PREVWEP = 128,
		IN_LEFT_ALT = 256,
		IN_RIGHT_ALT = 512
	};
	void updateInputs();
	void releaseKey(Inputs key);
	void pressKey(Inputs key);
	void checkKey(Inputs key);
private:
	int m_keylist = 0;
};

