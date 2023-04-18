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

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_currRasterizerState;			// 현재 레스터 상태
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_solidRasterizerState;			// 채우는 레스터 상태
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_wireRasterizerState;			// 와이어 레스터 상태

	D3D_FEATURE_LEVEL		m_d3dFeatureLevels{};		// 피처 레벨 
	D3D11_TEXTURE2D_DESC	m_d3dBackBufferDesc{};		// 백버퍼 설정
	D3D11_VIEWPORT			m_d3dViewport{};			// 뷰포트

public:
	virtual HRESULT Initialize(HWND hWnd) override;
	virtual void Finalize() override;
	virtual HRESULT OnResize() override;
	virtual void Update() override;
	virtual void BeginDraw() override;
	virtual void EndDraw() override;

private:
	HRESULT CreateDevice();				// 디바이스, 디바이스 컨텍스트 만들기
	HRESULT CreateSwapChain();			// 스왑체인 설정
	HRESULT CreateRenderTargetView();	// 렌더타겟 설정
	HRESULT CreateDepthStencilView();	// 뎁스스탠실뷰 설정
	void SetCamera();
	
	HRESULT CreateRasterState();		// 레스터 상태 설정

	HRESULT CreateObject();

	// void Test();

#pragma region 오브젝트
	/// <summary>
	/// 일단 오브젝트 생성함
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

#pragma endregion 오브젝트

private:
	Matrix4x4 m_world;			// 월드 매트릭스
	Matrix4x4 m_view;			// 뷰 매트릭스
	Matrix4x4 m_projection;		// 투영 매트릭스

	DirectX::XMFLOAT4 m_pos;

	Camera camera;
};

