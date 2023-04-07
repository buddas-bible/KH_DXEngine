#pragma once
#include "DXDefine.h"
#include "Color.h"

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

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_currRasterizerState;			// 현재 레스터 상태
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_solidRasterizerState;			// 채우는 레스터 상태
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_wireRasterizerState;			// 와이어 레스터 상태

	D3D_FEATURE_LEVEL		m_d3dFeatureLevels{};		// 피처 레벨 
	D3D11_TEXTURE2D_DESC	m_d3dBackBufferDesc{};		// 백버퍼 설정
	D3D11_VIEWPORT			m_d3dViewport{};			// 뷰포트

public:
	virtual HRESULT Initialize(HWND hWnd) override;
	virtual void Finalize() override;
	virtual void Update() override;
	virtual void BeginDraw() override;
	virtual void EndDraw() override;

private:
	HRESULT CreateDevice();				// 디바이스, 디바이스 컨텍스트 만들기
	HRESULT CreateSwapChain();			// 스왑체인 설정
	HRESULT CreateRenderTargetView();	// 렌더타겟 설정
	HRESULT CreateDepthStencilView();	// 뎁스스탠실뷰 설정
	HRESULT CreateRasterState();		// 레스터 상태 설정

	HRESULT CreateObject();

	HRESULT CreateInputLayout();
	// void Test();

	HRESULT CreateCubeShaders();

#pragma region 오브젝트
	/// <summary>
	/// 일단 오브젝트 생성함
	/// </summary>
	/// <returns></returns>
private:
	/// <summary>
	/// 큐브 관련 무언가
	/// </summary>
	HRESULT CreateCube();
	UINT cubeIndexCount;					// 큐브 인덱스 개수

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_cubeVertexBuffer;		// 정점 버퍼		(정점들의 버퍼?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_cubeIndexBuffer;		// 인덱스 버퍼	(정점들의 인덱스?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_constantBuffer;	// 상수 버퍼		(아마 행렬)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_cubeInputLayout;

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_cubeEffect;			// 버텍스 셰이더 픽셀 셰이더를 대체할 무언가
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_cubeTechnique;		// 테크
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_cubeMatrixVariable;	// 상수버퍼를 대신할 무언가?
	
	Microsoft::WRL::ComPtr<ID3DX11EffectShaderResourceVariable> m_cubeShaderResource;	// 
	Microsoft::WRL::ComPtr<ID3DX11EffectSamplerVariable> m_cubeSampler;		// 어떻게 샘플링 할지. 기본값이 이미 들어있어서 설정을 따로 안해줘도 텍스쳐는 찍힘

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_cubeSamplerState;			/// 없어도 되는 듯

	Microsoft::WRL::ComPtr<ID3D11Resource>				m_cubeTexture;		/// 초기화 당시에만 필요하고 가지고 있을 필요는 없을듯
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_cubeTextureView;	// 텍스쳐 들고 있을 친구

private:
	HRESULT CreateAxis();
	UINT axisIndexCount;					// 축 인덱스 개수

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_axisVertexBuffer;		// 정점 버퍼		(정점들의 버퍼?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_axisIndexBuffer;		// 인덱스 버퍼	(정점들의 인덱스?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_axisInputLayout;

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_axisEffect;			// 버텍스 셰이더 픽셀 셰이더를 대체할 무언가
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_axisTechnique;		// 테크
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_axisMatrixVariable;	// 상수버퍼를 대신할 무언가?

private:
	HRESULT CreateGrid();
	UINT gridIndexCount;					// 그리드 인덱스 개수

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_gridVertexBuffer;		// 정점 버퍼		(정점들의 버퍼?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_gridIndexBuffer;		// 인덱스 버퍼	(정점들의 인덱스?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_gridInputLayout;

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_gridEffect;			// 버텍스 셰이더 픽셀 셰이더를 대체할 무언가
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_gridTechnique;		// 테크
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_gridMatrixVariable;	// 상수버퍼를 대신할 무언가?

#pragma endregion 오브젝트

private:
	// Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;		// 버텍스 셰이더
	// Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;		// 픽셀 셰이더


private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;				// 버텍스 위치 정보
		DirectX::XMFLOAT4 color;			// 버텍스 컬러 정보
	};

	struct TextureVertex
	{
		DirectX::XMFLOAT3 pos;				// 버텍스 위치 정보
		DirectX::XMFLOAT3 normal;			// 버텍스 노말 정보
		DirectX::XMFLOAT2 uv;				// 버텍스 텍스쳐 UV 좌표
		DirectX::XMFLOAT4 color;			// 버텍스 컬러 정보
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;			// 월드 매트릭스
		DirectX::XMMATRIX view;			// 뷰 매트릭스
		DirectX::XMMATRIX projection;		// 투영 매트릭스
	};

	DirectX::XMFLOAT4 m_x;
	DirectX::XMFLOAT4 m_z;
	DirectX::XMFLOAT4 m_y;

	DirectX::XMFLOAT4 m_pos;

	ConstantBuffer m_constantBufferData;	// 상수 버퍼 데이터 (월드 뷰 투영 매트릭스 정보)
};

