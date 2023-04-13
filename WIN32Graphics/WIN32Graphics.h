#pragma once
#include "windows.h"
#include "../CoreEngine/IGraphics.h"

class Vector2D;

class WIN32Graphics : public IGraphics
{
public:
	WIN32Graphics() = default;
	virtual ~WIN32Graphics();

private:
	HWND m_hWnd;
	RECT m_rect;
	PAINTSTRUCT m_ps;
	HBITMAP m_bitmap;
	HDC m_deviceContext;
	HDC m_backBuffer;

public:
	virtual HRESULT Initialize(HWND hWnd) override;
	virtual void Finalize() override;
	virtual HRESULT OnResize() override;
	virtual void Update() override;
	virtual void BeginDraw() override;
	virtual void EndDraw() override;

public:
	void DrawPixel(float _x, float _y, COLORREF _color = RGB(0, 0, 0));
	void DrawLine(float _sX, float _sY, float _eX, float _eY, COLORREF _color = RGB(0, 0, 0));
	void DrawTriangle(const Vector2D& _v1, const Vector2D& _v2, const Vector2D& _v3);

private:

};

/*
벡터 -> 스크린 좌표
데카르트 좌표계랑 스크린 좌표계는 영점과 Y 증가 방향이 다르다.
0, 0을 좌하단 기준으로 잡고 그리고 싶다...

(0, 0)은 (x(0), -y(0) + 스크린y사이즈)?


스크린 -> 벡터

(x + 0.5f, -(y + 0.5f) + 스크린y사이즈)?

*/