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

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_currRasterizerState;			// ���� ������ ����
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_solidRasterizerState;			// ä��� ������ ����
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_wireRasterizerState;			// ���̾� ������ ����

	D3D_FEATURE_LEVEL		m_d3dFeatureLevels{};		// ��ó ���� 
	D3D11_TEXTURE2D_DESC	m_d3dBackBufferDesc{};		// ����� ����
	D3D11_VIEWPORT			m_d3dViewport{};			// ����Ʈ

public:
	virtual HRESULT Initialize(HWND hWnd) override;
	virtual void Finalize() override;
	virtual void Update() override;
	virtual void BeginDraw() override;
	virtual void EndDraw() override;

private:
	HRESULT CreateDevice();				// ����̽�, ����̽� ���ؽ�Ʈ �����
	HRESULT CreateSwapChain();			// ����ü�� ����
	HRESULT CreateRenderTargetView();	// ����Ÿ�� ����
	HRESULT CreateDepthStencilView();	// �������ĽǺ� ����
	HRESULT CreateRasterState();		// ������ ���� ����

	HRESULT CreateObject();

	HRESULT CreateInputLayout();
	// void Test();

	HRESULT CreateCubeShaders();

#pragma region ������Ʈ
	/// <summary>
	/// �ϴ� ������Ʈ ������
	/// </summary>
	/// <returns></returns>
private:
	/// <summary>
	/// ť�� ���� ����
	/// </summary>
	HRESULT CreateCube();
	UINT cubeIndexCount;					// ť�� �ε��� ����

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_cubeVertexBuffer;		// ���� ����		(�������� ����?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_cubeIndexBuffer;		// �ε��� ����	(�������� �ε���?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_constantBuffer;	// ��� ����		(�Ƹ� ���)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_cubeInputLayout;

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_cubeEffect;			// ���ؽ� ���̴� �ȼ� ���̴��� ��ü�� ����
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_cubeTechnique;		// ��ũ
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_cubeMatrixVariable;	// ������۸� ����� ����?
	
	Microsoft::WRL::ComPtr<ID3DX11EffectShaderResourceVariable> m_cubeShaderResource;	// 
	Microsoft::WRL::ComPtr<ID3DX11EffectSamplerVariable> m_cubeSampler;		// ��� ���ø� ����. �⺻���� �̹� ����־ ������ ���� �����൵ �ؽ��Ĵ� ����

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_cubeSamplerState;			/// ��� �Ǵ� ��

	Microsoft::WRL::ComPtr<ID3D11Resource>				m_cubeTexture;		/// �ʱ�ȭ ��ÿ��� �ʿ��ϰ� ������ ���� �ʿ�� ������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_cubeTextureView;	// �ؽ��� ��� ���� ģ��

private:
	HRESULT CreateAxis();
	UINT axisIndexCount;					// �� �ε��� ����

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_axisVertexBuffer;		// ���� ����		(�������� ����?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_axisIndexBuffer;		// �ε��� ����	(�������� �ε���?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_axisInputLayout;

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_axisEffect;			// ���ؽ� ���̴� �ȼ� ���̴��� ��ü�� ����
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_axisTechnique;		// ��ũ
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_axisMatrixVariable;	// ������۸� ����� ����?

private:
	HRESULT CreateGrid();
	UINT gridIndexCount;					// �׸��� �ε��� ����

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_gridVertexBuffer;		// ���� ����		(�������� ����?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_gridIndexBuffer;		// �ε��� ����	(�������� �ε���?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_gridInputLayout;

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_gridEffect;			// ���ؽ� ���̴� �ȼ� ���̴��� ��ü�� ����
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_gridTechnique;		// ��ũ
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_gridMatrixVariable;	// ������۸� ����� ����?

#pragma endregion ������Ʈ

private:
	// Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;		// ���ؽ� ���̴�
	// Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;		// �ȼ� ���̴�


private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;				// ���ؽ� ��ġ ����
		DirectX::XMFLOAT4 color;			// ���ؽ� �÷� ����
	};

	struct TextureVertex
	{
		DirectX::XMFLOAT3 pos;				// ���ؽ� ��ġ ����
		DirectX::XMFLOAT3 normal;			// ���ؽ� �븻 ����
		DirectX::XMFLOAT2 uv;				// ���ؽ� �ؽ��� UV ��ǥ
		DirectX::XMFLOAT4 color;			// ���ؽ� �÷� ����
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;			// ���� ��Ʈ����
		DirectX::XMMATRIX view;			// �� ��Ʈ����
		DirectX::XMMATRIX projection;		// ���� ��Ʈ����
	};

	DirectX::XMFLOAT4 m_x;
	DirectX::XMFLOAT4 m_z;
	DirectX::XMFLOAT4 m_y;

	DirectX::XMFLOAT4 m_pos;

	ConstantBuffer m_constantBufferData;	// ��� ���� ������ (���� �� ���� ��Ʈ���� ����)
};

