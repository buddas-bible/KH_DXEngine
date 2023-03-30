#pragma once

#include "DXDefine.h"

class DXGraphics : public IGraphics
{
public:
	DXGraphics() = default;
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

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_pd3dInputLayout;

	D3D_FEATURE_LEVEL		m_d3dFeatureLevels{};		// 피처 레벨 
	D3D11_TEXTURE2D_DESC	m_d3dBackBufferDesc{};		// 백버퍼 설정
	D3D11_VIEWPORT			m_d3dViewport{};			// 뷰포트

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_vertexBuffer;		// 정점 버퍼		(정점들의 버퍼?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_indexBuffer;		// 인덱스 버퍼	(정점들의 인덱스?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_constantBuffer;	// 상수 버퍼		(아마 행렬)

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;

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

	HRESULT CreateShaders();

	/// <summary>
	/// 그리드
	/// </summary>
	HRESULT DrawGrid();

private:
	Microsoft::WRL::ComPtr<ID3DX11Effect> m_effect;
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique> m_effectTechnique;
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_effectMatrixVariable;

private:
	struct VertexCombined
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 worldTM;
		DirectX::XMFLOAT4X4 viewTM;
		DirectX::XMFLOAT4X4 projTM;
	};

	ConstantBuffer m_constantBufferData;
	UINT count;
};

