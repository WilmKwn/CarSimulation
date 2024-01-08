#include "Input.h"

Input::Input()
{

}

Input::~Input()
{

}

bool Input::GetKeyDown(int vk_code)
{
	return GetAsyncKeyState(vk_code) & 0x8000;
}