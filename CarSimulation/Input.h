#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class Input
{
public:
	Input();
	~Input();

	static bool GetKeyDown(int vk_code);

private:
};