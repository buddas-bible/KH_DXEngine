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
	/// ���� ���� ������ ������..
	DXGraphics* m_pDXGraphicsEngine{};
	WIN32Graphics* m_pWINGraphicsEngine{};

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
	void Run();
};

