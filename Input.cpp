#include "Input.h"

void Input::SetKeyDown(int key)
{
	if (key >= 0)
		keys_[key] = true;
}

void Input::SetKeyUp(int key)
{
	if (key >= 0)
		keys_[key] = false;
}

bool Input::IsKeyDown(int key)
{
	if (key >= 0)
		return keys_[key];
	else
		return false;
}
