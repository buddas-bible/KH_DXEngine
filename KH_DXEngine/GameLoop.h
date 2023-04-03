#pragma once
#include <windows.h>
// #include "../CoreEngine/CoreEngine.h"

class CoreEngine;

class GameLoop
{
private:
	HWND m_hWnd;
	MSG m_msg;

private:
	CoreEngine* m_pCoreEngine;

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
	void Render();
	void Update();
	void Run();
};

