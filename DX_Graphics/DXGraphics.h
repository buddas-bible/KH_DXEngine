#pragma once
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#pragma comment(lib, "dxgi.lib")

#include <wrl.h>

#include "../CoreEngine/IGraphics.h"
#include "../KH_Math/Vector3D.h"

class DXGraphics : public IGraphics
{
public:
	DXGraphics();
	virtual ~DXGraphics();

private:
	HWND m_hWnd;
	
	Microsoft::WRL::ComPtr<ID3D11Device>			m_pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pd3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>			m_pDXGISwapChain1;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_backBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pd3dRenderTargetView;
	
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pd3dDepthStencilView;

	D3D_FEATURE_LEVEL		m_d3dFeatureLevels{};
	D3D11_TEXTURE2D_DESC	m_d3dBackBufferDesc{};
	D3D11_VIEWPORT			m_d3dViewport{};

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_constantBuffer;

public:
	virtual HRESULT Initialize(HWND hWnd) override;
	virtual void Finalize() override;
	virtual void BeginDraw() override;
	virtual void EndDraw() override;

private:
	HRESULT CreateDevice();
	HRESULT CreateSwapChain();
	HRESULT CreateRenderTargetView();
	HRESULT CreateDepthStencilView();
	HRESULT CreateResouce();
	void Test();

private:
	struct VertexCombined
	{
		Vector3D position;
		Vector3D color;
	};
};

