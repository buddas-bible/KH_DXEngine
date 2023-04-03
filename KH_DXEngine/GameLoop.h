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
	// �ʱ�ȭ
	HRESULT Initialize(HINSTANCE hInstance);
	
	// ����
	void Loop();

	// �Ҵ� ���� ���...
	void Finalize();


	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void Render();
	void Update();
	void Run();
};

