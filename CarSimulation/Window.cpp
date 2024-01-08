#include "Window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

Window::Window(int width, int height, HINSTANCE hInstance) 
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"CarSimulation";
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx(&wcex);

	m_handle = CreateWindowEx(NULL, L"CarSimulation", L"CarSimulation", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, width, height, NULL, NULL, hInstance, NULL);
}

Window::~Window() 
{
}

std::pair<int, int> Window::GetSize()
{
	RECT r;
	GetClientRect(m_handle, &r);
	return std::make_pair<int, int>((int)r.right, (int)r.bottom);
}

HWND Window::GetHandle()
{
	return m_handle;
}