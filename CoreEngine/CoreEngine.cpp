#include "CoreEngine.h"

#include "IGraphics.h"
#include "../DX_Graphics/DXGraphics.h"
#include "../WIN32Graphics/WIN32Graphics.h"

CoreEngine::CoreEngine() : m_pGraphics()
{

}

CoreEngine::~CoreEngine()
{

}

HRESULT CoreEngine::Initialize(HWND hWnd)
{
	HRESULT hr = S_OK;
	
	m_pGraphics = new DXGraphics;
	// m_pGraphics = new WIN32Graphics;
	hr = m_pGraphics->Initialize(hWnd);

	return hr;
}

void CoreEngine::Finalize()
{
	if (m_pGraphics != nullptr)
	{
		delete m_pGraphics;
	}
}

void CoreEngine::Render()
{
	m_pGraphics->BeginDraw();

	m_pGraphics->EndDraw();
}

void CoreEngine::Update()
{

}
