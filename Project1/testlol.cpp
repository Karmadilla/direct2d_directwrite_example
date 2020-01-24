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
				reinterpret_cast<IUnknown**>(&pDWriteFactory_)
			);
		}
	}

	wszText_ = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque eu justo vel arcu iaculis ultrices. Nullam nisl quam, dictum at augue sed, luctus pharetra massa. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Morbi imperdiet suscipit vestibulum. Sed euismod, velit blandit fringilla lacinia, est nunc finibus ipsum, sodales tincidunt tortor erat sed ex. Proin nisl arcu, viverra sagittis lectus nec, ultricies dignissim arcu. Sed sit amet dolor vel purus fringilla condimentum. Sed luctus rutrum justo, quis vestibulum massa varius non. Sed eu dolor erat. Mauris eget nibh justo. Phasellus nec risus a sapien tincidunt mattis id et nisl. In vitae odio ac nunc convallis tincidunt. Fusce eget tellus at enim egestas molestie. Vestibulum tortor nisi, porta eu luctus id, placerat et mi. Aliquam auctor aliquet lacus. Nulla ornare ipsum facilisis magna facilisis lobortis.";
	cTextLength_ = (UINT32)wcslen(wszText_);

	if (SUCCEEDED(hr))
	{
		hr = pDWriteFactory_->CreateTextFormat(
			L"Oswald",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.0f,
			L"en-us", &pTextFormat_);
	}

	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	}

	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	return hr;
}

HRESULT CreateDeviceResources(HWND hwnd_, RECT rc)
{
	HRESULT hr;

	GetClientRect(hwnd_, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	if (!pRT_)
	{
		D2D1_HWND_RENDER_TARGET_PROPERTIES hRTP = D2D1::HwndRenderTargetProperties(hwnd_, size);
		
		hr = pD2DFactory_->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(), hRTP, &pRT_);
	}

	if (SUCCEEDED(hr))
	{
		hr = pRT_->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&pBlackBrush_);
	}

	return hr;
}

void DrawSomething(RECT rc, HWND hWnd)
{
	HRESULT hr;

	GetClientRect(hWnd, &rc);

	HDC screen = GetDC(hWnd);
	float dpiScaleX_ = GetDeviceCaps(screen, LOGPIXELSX) / 96.0f;
	float dpiScaleY_ = GetDeviceCaps(screen, LOGPIXELSY) / 96.0f;

	D2D1_RECT_F layoutRect = D2D1::RectF(
		static_cast<FLOAT>(rc.left) / dpiScaleX_,
		static_cast<FLOAT>(rc.top) / dpiScaleY_,
		static_cast<FLOAT>(rc.right) / dpiScaleX_,
		static_cast<FLOAT>(rc.bottom) / dpiScaleY_);
	pRT_->BeginDraw();
	pRT_->Clear(D2D1::ColorF(D2D1::ColorF::White));
	pRT_->DrawText(wszText_, cTextLength_, pTextFormat_, layoutRect, pBlackBrush_);
	hr = pRT_->EndDraw();
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

	RECT rc = { 0 };

	if (!RegisterClassExW(&wcex))
	{
		return 1;
	}

	HWND hWnd = CreateWindowW(wcex.lpszClassName, L"First Window", WS_OVERLAPPEDWINDOW, 50, 50, 1280, 720, nullptr, nullptr, hInstance, nullptr);

	HRESULT hr = CreateDeviceIndependentResources();

	hr = CreateDeviceResources(hWnd, rc);	

	ShowWindow(hWnd, nCmdShow);	
	UpdateWindow(hWnd);

	if (SUCCEEDED(hr))
	{
		DrawSomething(rc, hWnd);
	}

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