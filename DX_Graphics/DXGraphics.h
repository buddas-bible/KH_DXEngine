#pragma once
#include "DXDefine.h"
#include "Color.h"
#include "Camera.h"

#include "TimeManager.h"

class Axis;
class Grid;

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
	virtual HRESULT OnResize() override;
	virtual void Update() override;
	virtual void BeginDraw() override;
	virtual void EndDraw() override;

private:
	HRESULT CreateDevice();				// ����̽�, ����̽� ���ؽ�Ʈ �����
	HRESULT CreateSwapChain();			// ����ü�� ����
	HRESULT CreateRenderTargetView();	// ����Ÿ�� ����
	HRESULT CreateDepthStencilView();	// �������ĽǺ� ����
	void SetCamera();
	
	HRESULT CreateRasterState();		// ������ ���� ����

	HRESULT CreateObject();

	HRESULT CreateInputLayout();

	HRESULT CreateCubeShaders();
	// void Test();

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

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_cubeInputLayout;

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_cubeEffect;			// ���ؽ� ���̴� �ȼ� ���̴��� ��ü�� ����
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_cubeTechnique;		// ��ũ
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_cubeMatrixVariable;	// ������۸� ����� ����?
	
	Microsoft::WRL::ComPtr<ID3DX11EffectShaderResourceVariable> m_cubeShaderResource;	// 
	Microsoft::WRL::ComPtr<ID3DX11EffectSamplerVariable> m_cubeSampler;		// ��� ���ø� ����. �⺻���� �̹� ����־ ������ ���� �����൵ �ؽ��Ĵ� ����

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_cubeSamplerState;			/// ��� �Ǵ� ��

	Microsoft::WRL::ComPtr<ID3D11Resource>				m_cubeTexture;		/// �ʱ�ȭ ��ÿ��� �ʿ��ϰ� ������ ���� �ʿ�� ������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_cubeTextureView;	// �ؽ��� ��� ���� ģ��

	Microsoft::WRL::ComPtr<ID3DX11EffectVectorVariable> m_directionalLight;

private:
	Axis* axis;
	HRESULT CreateAxis();

private:
	Grid* grid;
	HRESULT CreateGrid();
	
#pragma endregion ������Ʈ

private:
	Matrix4x4 m_world;			// ���� ��Ʈ����
	Matrix4x4 m_view;			// �� ��Ʈ����
	Matrix4x4 m_projection;		// ���� ��Ʈ����

	DirectX::XMFLOAT4 m_pos;

	Camera camera;
	Vector3D directionalLight;
	Vector3D pointLight;
};

