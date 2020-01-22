#ifndef UNICODE
#define UNICODE
#endif


#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "testtest.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"First Window";
	wcex.lpfnWndProc = WndProc;

	if (!RegisterClassExW(&wcex))
	{
		return 1;
	}

	HWND hWnd = CreateWindow(wcex.lpszClassName, L"First Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}