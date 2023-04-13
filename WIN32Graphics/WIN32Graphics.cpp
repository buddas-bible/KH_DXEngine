#include "WIN32Graphics.h"

#include "../KH_Math/Vector2D.h"

WIN32Graphics::~WIN32Graphics()
{

}

HRESULT WIN32Graphics::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	m_deviceContext = GetDC(m_hWnd);

	GetClientRect(m_hWnd, &m_rect);
	m_backBuffer = CreateCompatibleDC(m_deviceContext);
	CreateCompatibleBitmap(m_deviceContext, m_rect.right, m_rect.bottom);
	SelectObject(m_backBuffer, m_bitmap);


	return S_OK;
}

void WIN32Graphics::Finalize()
{

}


HRESULT WIN32Graphics::OnResize()
{
	return 0;
}

void WIN32Graphics::Update()
{

}

void WIN32Graphics::BeginDraw()
{
	PatBlt(m_backBuffer, 0, 0, m_rect.right, m_rect.bottom, BLACKNESS);


	DrawTriangle({ 10, 10 }, { 800, 10 }, { 800, 800 });
}

void WIN32Graphics::EndDraw()
{
	BitBlt(m_deviceContext, 0, 0, m_rect.right, m_rect.bottom, m_backBuffer, 0, 0, SRCCOPY);
}

void WIN32Graphics::DrawPixel(float _x, float _y, COLORREF _color)
{
	int i = ::SetPixel(m_deviceContext, _x, _y, _color);
}

void WIN32Graphics::DrawLine(float _sX, float _sY, float _eX, float _eY, COLORREF _color /*= RGB(0, 255, 0)*/)
{
	// x 증가량
	int w = _eX - _sX;
	// y 증가량
	int h = _eY - _sY;

	// 어느 방향으로 진행하는지 부호를 붙여줌
	int signX = (w >= 0) ? 1 : -1;
	int signY = (h >= 0) ? 1 : -1;
	
	// 판별식에 쓰일 데이터
	int dx = w * signX;
	int dy = h * signY;

	/*
	픽셀과 픽셀 중간값 0.5를 이용해서
	직선의 방정식에 다음 위치의 x 값을 넣었을 때, y < y0 + 0.5이 성립한다면
	y 를 증가시키지 않고 수평 이동 후 픽셀을 찍는다. (y 값이 픽셀 중간값을 넘지 않았기 때문에)
	
	직선의 방정식을 이용해서 식을 단순화 시켜서 나온 판별식으로 픽셀 위치를 계산할 수 있다.
	
	판별식의 패턴이 있는데 예를 들면,
	x 가 증가하면 2h 증가, y 가 증가하면 2w 감소
	x 만 증가한다면 2h. x,y 같이 증가한다면 2h-2w
	초기 판별식에 위와 유사한 증감 패턴을 더하는 것으로 픽셀을 찍을 수 있을 것이다.
	*/

	int aw, ah;			// 너비 높이 절댓값
	int f, dfx, dfy;	// 판별식

	if (w >= 0)			// 기울기 크기를 위해 절댓값을 구함
		aw = w;			// w 가 크면 x 를 증가시키면서 픽셀을 찍어나감
	else				// 반대로 h 가 크면 y를 증가시키면서 픽셀을 찍어나감
		aw = -w;
	if (h >= 0)
		ah = h;
	else
		ah = -h;

	// 기울기가 큰지 작은지? x 를 증가시키면서 갈지 y 를 증가시키면서 갈지 판단 하는 것.
	bool isXY = (aw >= ah);
	f = isXY ? 2 * dy - dx : 2 * dx - dy;
	dfx = isXY ? 2 * dy : 2 * dx;
	dfy = isXY ? 2 * (dy - dx) : 2 * (dx - dy);

	// 라인 시작 점
	int x = _sX;
	int y = _sY;

	// 기울기가 작으므로 x 를 증가시키면서 선을 그림
	if (isXY)
	{
		while (x != _eX)
		{
			SetPixel(m_deviceContext, x, y, _color);
			if (f < 0)			// 판별식이 0보다 작다면, 
			{
				f += dfx;		// 판별식 x 값(2h)은 평행이동함
			}
			else				// 판별식이 0보다 크다면
			{
				f += dfy;		// 판별식 x, y 값(2h-2w)을 증가시킴
				y += signY;		// 픽셀 y 증가
			}

			x += signX;			// 픽셀 x 값 증가
		}
	}

	// 기울기가 크므로 y 를 증가시키면서 선을 그림
	else
	{
		while (y != _eY)
		{
			SetPixel(m_deviceContext, x, y, _color);
			if (f < 0)			// 판별식이 0보다 작다면, 
			{
				f += dfx;		// 판별식 y 값(2w)은 평행이동함
			}
			else				// 판별식이 0보다 크다면
			{
				f += dfy;		// 판별식 x, y 값(2w-2h)을 증가시킴
				x += signX;		// 픽셀 x 증가
			}

			y += signY;			// 픽셀 y 값 증가
		}
	}
}

void WIN32Graphics::DrawTriangle(const Vector2D& _v1, const Vector2D& _v2, const Vector2D& _v3)
{
	DrawLine(_v1.x, _v1.y, _v2.x, _v2.y);
	DrawLine(_v2.x, _v2.y, _v3.x, _v3.y);
	DrawLine(_v3.x, _v3.y, _v1.x, _v1.y);
}
