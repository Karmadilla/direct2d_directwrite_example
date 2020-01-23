#ifndef UNICODE
#define UNICODE
#endif

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "testtest.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT CreateDeviceIndependentResources()
{
	HRESULT hr;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory_);

	if (SUCCEEDED(hr))
	{
		if (SUCCEEDED(hr))
		{
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown **>(&pDWriteFactory_)
			);
		}
	}

	wszText_ = L"Hello World";
	cTextLength_ = (UINT32)wszText_;

	return hr;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex = { 0 };
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

	CreateDeviceIndependentResources();

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