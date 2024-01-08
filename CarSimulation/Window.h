#pragma once

#include <iostream>

#include "Input.h"

class Window
{
public:
	Window(int width, int height, HINSTANCE hInstance);
	~Window();

	HWND GetHandle();

	std::pair<int, int> GetSize();

private:
	HWND m_handle;

	std::pair<int, int> size;
};