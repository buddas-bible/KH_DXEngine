#include "InputManager.h"

#include "windows.h"

InputManager::InputManager()
{
	Update();
}

InputManager::~InputManager()
{

}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::Update()
{
	for (size_t keycode = 0x01; keycode < 0xFF; keycode++)
	{
		past[keycode] = current[keycode];
		current[keycode] = GetAsyncKeyState(keycode) & 0x8000;
	}
}

bool InputManager::PressedKey(int keycode)
{
	return current[keycode];
}

bool InputManager::PressedKeyNow(int keycode)
{
	return current[keycode] && !past[keycode];
}

bool InputManager::ReleaseKey(int keycode)
{
	return !current[keycode];
}

bool InputManager::ReleaseKeyNow(int keycode)
{
	return !current[keycode] && past[keycode];
}
