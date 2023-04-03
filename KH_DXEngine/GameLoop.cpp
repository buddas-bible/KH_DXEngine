#include "GameLoop.h"

#include "../CoreEngine/CoreEngine.h"
#pragma comment(lib, "KH_Math.lib")
#include <cmath>

HRESULT GameLoop::SetWindow(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GameLoop::WndProc; // 프로시저
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 0);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"KH_Engine";
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);

	m_hWnd = CreateWindowW(
		wcex.lpszClassName,
		L"KH_Engine",
		WS_OVERLAPPEDWINDOW | WS_BORDER | WS_SYSMENU,
		CW_USEDEFAULT,		// 윈도우 좌측 상단의 x 좌표
		CW_USEDEFAULT,					// 윈도우 좌측 상단의 y 좌표
		0,		// 윈도우 가로 방향 해상도
		0,					// 윈도우 세로 방향 해상도
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!m_hWnd)
	{
		return S_FALSE;
	}

	float dpi = GetDpiForWindow(m_hWnd);

	SetWindowPos(
		m_hWnd,
		NULL,
		NULL,
		NULL,
		static_cast<int>(ceil(1600.f * dpi / 96.f)),
		static_cast<int>(ceil(900.f * dpi / 96.f)),
		SWP_NOMOVE);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	return S_OK;
}

HRESULT GameLoop::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = S_OK;

	// 윈도우 창 설정
	hr = SetWindow(hInstance);
	if (FAILED(hr))
	{
		return hr;
	}

	// 그래픽 엔진 설정
	m_pCoreEngine = new CoreEngine();
	hr = m_pCoreEngine->Initialize(m_hWnd);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void GameLoop::Loop()
{
	while (true)
	{
		if (PeekMessage(&m_msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)	break;
			DispatchMessage(&m_msg);
		}
		else
		{
			Run();
		}
	}
}

void GameLoop::Finalize()
{
	if (m_pCoreEngine != nullptr)
	{
		// delete m_pCoreEngine;
		// m_pCoreEngine = nullptr;
	}
}

LRESULT CALLBACK GameLoop::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void GameLoop::Render()
{
	m_pCoreEngine->Render();
}

void GameLoop::Update()
{
	m_pCoreEngine->Update();
}

void GameLoop::Run()
{
	Update();
	Render();
}