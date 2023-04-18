#pragma once
#include "DXDefine.h"
#include "KHColor.h"
#include "Camera.h"

#include "TimeManager.h"

#include <vector>

class Box;
class Axis;
class Grid;
class Skull;
class MeshObject;

class CASEParser;
class MeshObject;

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

	// void Test();

#pragma region ������Ʈ
	/// <summary>
	/// �ϴ� ������Ʈ ������
	/// </summary>
	/// <returns></returns>
private:
	Box* box;
	HRESULT CreateCube();

private:
	Axis* axis;
	HRESULT CreateAxis();

private:
	Grid* grid;
	HRESULT CreateGrid();

private:
	Skull* skull;
	HRESULT CreateSkull();
	

	CASEParser* m_parser;
	// Mash* m_genji;

	std::vector<MeshObject*> m_objectList;
	HRESULT CreateMeshObject();

#pragma endregion ������Ʈ

private:
	Matrix4x4 m_world;			// ���� ��Ʈ����
	Matrix4x4 m_view;			// �� ��Ʈ����
	Matrix4x4 m_projection;		// ���� ��Ʈ����

	DirectX::XMFLOAT4 m_pos;

	Camera camera;
};

