#pragma once
#include <windows.h>

class DXGraphics;
class WIN32Graphics;

class GameLoop
{
private:
	HWND m_hWnd;
	MSG m_msg;

private:
	/// 엔진 관련 포인터 변수들..
	DXGraphics* m_pDXGraphicsEngine{};
	WIN32Graphics* m_pWINGraphicsEngine{};

private:
	HRESULT SetWindow(HINSTANCE hInstance);

public:
	// 초기화
	HRESULT Initialize(HINSTANCE hInstance);
	
	// 루프
	void Loop();

	// 할당 해제 등등...
	void Finalize();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void Run();
};

