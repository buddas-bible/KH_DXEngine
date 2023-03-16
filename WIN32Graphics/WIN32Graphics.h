#pragma once

#include <windows.h>

class Vector2D;

class WIN32Graphics
{
public:
	WIN32Graphics();
	~WIN32Graphics();

private:
	HWND m_hWnd;
	RECT m_rect;
	PAINTSTRUCT m_ps;
	HBITMAP m_bitmap;
	HDC m_deviceContext;
	HDC m_backBuffer;

public:
	HRESULT Initialize(HWND hWnd);
	void BeginDraw();
	void EndDraw();

public:
	void DrawPixel(float _x, float _y, COLORREF _color = RGB(0, 0, 0));
	void DrawLine(float _sX, float _sY, float _eX, float _eY, COLORREF _color = RGB(0, 0, 0));
	void DrawTriangle(const Vector2D& _v1, const Vector2D& _v2, const Vector2D& _v3);

private:

};

/*
���� -> ��ũ�� ��ǥ
��ī��Ʈ ��ǥ��� ��ũ�� ��ǥ��� ������ Y ���� ������ �ٸ���.
0, 0�� ���ϴ� �������� ��� �׸��� �ʹ�...

(0, 0)�� (x(0), -y(0) + ��ũ��y������)?


��ũ�� -> ����

(x + 0.5f, -(y + 0.5f) + ��ũ��y������)?

*/