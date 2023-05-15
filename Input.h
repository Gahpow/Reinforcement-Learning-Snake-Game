#pragma once


class Input
{
private:
	bool keys_[256]{ 0 };
public:
	void SetKeyDown(int key);
	void SetKeyUp(int key);
	bool IsKeyDown(int key);
};

