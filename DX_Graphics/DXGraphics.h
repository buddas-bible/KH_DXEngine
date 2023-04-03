#pragma once
#include "DXDefine.h"
#include "Color.h"

// #include "../DXTK/Inc/d3dx11effect.h"
// #include "../DXTK/Inc/Effects.h"

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

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_solidRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_wireRasterizerState;



	D3D_FEATURE_LEVEL		m_d3dFeatureLevels{};		// 피처 레벨 
	D3D11_TEXTURE2D_DESC	m_d3dBackBufferDesc{};		// 백버퍼 설정
	D3D11_VIEWPORT			m_d3dViewport{};			// 뷰포트

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_vertexBuffer;		// 정점 버퍼		(정점들의 버퍼?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_indexBuffer;		// 인덱스 버퍼	(정점들의 인덱스?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_constantBuffer;	// 상수 버퍼		(아마 행렬)

public:
	virtual HRESULT Initialize(HWND hWnd) override;
	virtual void Finalize() override;
	virtual void BeginDraw() override;
	virtual void EndDraw() override;

private:
	HRESULT CreateDevice();				// 디바이스, 디바이스 컨텍스트 만들기
	HRESULT CreateSwapChain();			// 스왑체인 설정
	HRESULT CreateRenderTargetView();	// 렌더타겟 설정
	HRESULT CreateDepthStencilView();	// 뎁스스탠실뷰 설정
	HRESULT CreateRasterState();		// 레스터 상태 설정

	HRESULT CreateObject();
	void Test();

	HRESULT CreateShaders();

	/// <summary>
	/// 그리드
	/// </summary>
	HRESULT DrawGrid();


private:
	// Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;		// 버텍스 셰이더
	// Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;		// 픽셀 셰이더


	Microsoft::WRL::ComPtr<ID3DX11Effect> m_effect;								// 버텍스 셰이더 픽셀 셰이더를 대체할 무언가
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique> m_effectTechnique;			// 테크
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_effectMatrixVariable;	// 

private:
	struct VertexCombined
	{
		DirectX::XMFLOAT3 pos;				// 버텍스 위치 정보
		// DirectX::XMFLOAT3 normal;		// 버텍스 노말 정보
		// DirectX::XMFLOAT2 uv;			// 버텍스 텍스쳐 UV 좌표
		DirectX::XMFLOAT4 color;			// 버텍스 컬러 정보
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;			// 월드 매트릭스
		DirectX::XMMATRIX view;			// 뷰 매트릭스
		DirectX::XMMATRIX projection;		// 투영 매트릭스
	};

	ConstantBuffer m_constantBufferData;	// 상수 버퍼 데이터 (월드 뷰 투영 매트릭스 정보)
	UINT count;								// 인덱스 개수
};

