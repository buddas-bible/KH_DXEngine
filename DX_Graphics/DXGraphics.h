#pragma once
// #include "../IGraphics/IGraphics.h"

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#pragma comment(lib, "dxgi.lib")

#include <wrl.h>

#include <winrt/base.h>
#pragma comment(lib, "windowsapp.lib")

// using namespace Microsoft::WRL;


class DXGraphics
{
public:
	DXGraphics();
	~DXGraphics();

private:
	HWND m_hWnd;
	
	Microsoft::WRL::ComPtr<ID3D11Device>			m_pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pd3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>			m_pDXGISwapChain1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pd3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_backBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pd3dDepthStencilView;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_constantBuffer;
	// ConstantBuffer m_constantBufferData;

public:
	HRESULT Initialize(HWND hWnd);
	void BeginDraw() ;
	void EndDraw();

private:
	HRESULT CreateDevice();
	HRESULT CreateSwapChain();
	HRESULT CreateRenderTargetView();
	HRESULT CreateDepthStencilView();
	void Test();

private:
	// winrt::agile_ref<winrt::Windows::UI::Core::CoreWindow> m_window;
};

