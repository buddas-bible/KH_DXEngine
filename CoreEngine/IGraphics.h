#pragma once
#include "windows.h"

class IGraphics abstract
{
public:
	IGraphics() {};
	virtual ~IGraphics() {};

public:
	virtual HRESULT Initialize(HWND hWnd)abstract;
	virtual void Finalize() abstract;
	virtual HRESULT OnResize() abstract;
	virtual void Update() abstract;
	virtual void BeginDraw() abstract;
	virtual void EndDraw() abstract;
};
