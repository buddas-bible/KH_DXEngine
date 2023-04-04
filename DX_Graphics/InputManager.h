#pragma once

class InputManager
{
private:
	InputManager();
	~InputManager();

	char current[255];
	char past[255];

public:
	static InputManager& GetInstance();

	void Update();

	bool PressedKey(int);
	bool PressedKeyNow(int);
	bool ReleaseKey(int);
	bool ReleaseKeyNow(int);
};

