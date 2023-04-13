#pragma once
#include "windows.h"

class IGraphics;

class CoreEngine
{
public:
	CoreEngine();
	~CoreEngine();

private:
	IGraphics* m_pGraphics;

public:
	HRESULT Initialize(HWND hWnd);
	void Finalize();
	HRESULT OnResize();
	void Render();
	void Update();
};