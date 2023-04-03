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



	D3D_FEATURE_LEVEL		m_d3dFeatureLevels{};		// ��ó ���� 
	D3D11_TEXTURE2D_DESC	m_d3dBackBufferDesc{};		// ����� ����
	D3D11_VIEWPORT			m_d3dViewport{};			// ����Ʈ

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_vertexBuffer;		// ���� ����		(�������� ����?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_indexBuffer;		// �ε��� ����	(�������� �ε���?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_constantBuffer;	// ��� ����		(�Ƹ� ���)

public:
	virtual HRESULT Initialize(HWND hWnd) override;
	virtual void Finalize() override;
	virtual void BeginDraw() override;
	virtual void EndDraw() override;

private:
	HRESULT CreateDevice();				// ����̽�, ����̽� ���ؽ�Ʈ �����
	HRESULT CreateSwapChain();			// ����ü�� ����
	HRESULT CreateRenderTargetView();	// ����Ÿ�� ����
	HRESULT CreateDepthStencilView();	// �������ĽǺ� ����
	HRESULT CreateRasterState();		// ������ ���� ����

	HRESULT CreateObject();
	void Test();

	HRESULT CreateShaders();

	/// <summary>
	/// �׸���
	/// </summary>
	HRESULT DrawGrid();


private:
	// Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;		// ���ؽ� ���̴�
	// Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;		// �ȼ� ���̴�


	Microsoft::WRL::ComPtr<ID3DX11Effect> m_effect;								// ���ؽ� ���̴� �ȼ� ���̴��� ��ü�� ����
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique> m_effectTechnique;			// ��ũ
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_effectMatrixVariable;	// 

private:
	struct VertexCombined
	{
		DirectX::XMFLOAT3 pos;				// ���ؽ� ��ġ ����
		// DirectX::XMFLOAT3 normal;		// ���ؽ� �븻 ����
		// DirectX::XMFLOAT2 uv;			// ���ؽ� �ؽ��� UV ��ǥ
		DirectX::XMFLOAT4 color;			// ���ؽ� �÷� ����
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;			// ���� ��Ʈ����
		DirectX::XMMATRIX view;			// �� ��Ʈ����
		DirectX::XMMATRIX projection;		// ���� ��Ʈ����
	};

	ConstantBuffer m_constantBufferData;	// ��� ���� ������ (���� �� ���� ��Ʈ���� ����)
	UINT count;								// �ε��� ����
};

