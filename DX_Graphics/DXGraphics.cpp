#include "DXGraphics.h"

#include <fstream>
#include <vector>

#include "Box.h"
#include "Axis.h"
#include "Grid.h"
#include "Skull.h"
#include "MeshObject.h"

#include "CASEParser.h"

using namespace Microsoft::WRL;

HRESULT DXGraphics::Initialize(HWND hwnd)
{
	m_hWnd = hwnd;

	HRESULT hr = S_OK;

	hr = CreateDevice();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"디바이스 생성 실패", L"초기화 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateSwapChain();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"스왑체인 생성 실패", L"초기화 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateRenderTargetView();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"랜더타겟뷰 생성 실패", L"초기화 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateDepthStencilView();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"뎁스스텐실뷰 생성 실패", L"초기화 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateRasterState();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"레스터스테이트 생성 실패", L"초기화 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	/// 이부분 부터는 그냥 바뀔수가 있음
	hr = CreateObject();
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void DXGraphics::Finalize()
{
	if (axis != nullptr)
	{
		delete axis;
	}
	if (grid != nullptr)
	{
		delete grid;
	}
	if (box != nullptr)
	{
		delete box;
	}
	if (skull != nullptr)
	{
		delete skull;
	}
	if (m_parser != nullptr)
	{
		delete m_parser;
	}
}

HRESULT DXGraphics::OnResize()
{
	HRESULT hr = S_OK;

	int ref = 0;
	ref = m_backBuffer.Reset();
	ref = m_pd3dRenderTargetView.Reset();
	ref = m_pDepthStencil.Reset();
	ref = m_pd3dDepthStencilView.Reset();

	DXGI_SWAP_CHAIN_DESC1 scDesc;
	hr = m_pDXGISwapChain1->GetDesc1(&scDesc);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pDXGISwapChain1->ResizeBuffers(
		NULL,
		NULL,
		NULL,
		DXGI_FORMAT_UNKNOWN,
		0
	);
	if (FAILED(hr))
	{
		return hr;
	}
	
	hr = CreateRenderTargetView();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateDepthStencilView();
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void DXGraphics::Update()
{
	static TimeManager& time = TimeManager::GetInstance();

	time.Update();
	camera.Update();

	m_view = camera.GetViewMatrix();
	m_projection = camera.GetProjMatrix();
	grid->Update(m_view, m_projection);
	axis->Update(m_view, m_projection);
	// box->Update(m_view, m_projection);
	// skull->Update(m_view, m_projection);
	for (auto& e : m_objectList)
	{
		e->Update(m_view, m_projection);
	}

	float dt = time.GetfDeltaTime();
	if (GetAsyncKeyState(VK_LEFT))
	{
		camera.Yaw(4 * dt);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		camera.Yaw(-4 * dt);
	}
	if (GetAsyncKeyState(VK_UP))
	{
		camera.Pitch(4 * dt);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		camera.Pitch(-4 * dt);
	}

	if (GetAsyncKeyState(VK_A))
	{
		camera.moveR(-4 * dt);
	}
	if (GetAsyncKeyState(VK_D))
	{
		camera.moveR(4 * dt);
	}
	if (GetAsyncKeyState(VK_W))
	{
		camera.moveL(4 * dt);
	}
	if (GetAsyncKeyState(VK_S))
	{
		camera.moveL(-4 * dt);
	}
	if (GetAsyncKeyState(VK_E))
	{
		camera.moveY(3.f * dt);
	}
	if (GetAsyncKeyState(VK_Q))
	{
		camera.moveY(-3.f * dt);
	}

	if (GetAsyncKeyState(VK_F1))
	{
		m_solidRasterizerState.As(&m_currRasterizerState);
	}
	if (GetAsyncKeyState(VK_F2))
	{
		m_wireRasterizerState.As(&m_currRasterizerState);
	}
	if (GetAsyncKeyState(VK_F5))
	{
		camera.SetPerspectiveView();
	}
	if (GetAsyncKeyState(VK_F6))
	{
		camera.SetOrthographicView();
	}
}

HRESULT DXGraphics::CreateDevice()
{
	HRESULT hr = S_OK;

	// 디바이스 기능 가져오기
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL FeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// 임시 포인터 생성
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	// 디바이스, 디바이스 컨텍스트 생성
	hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		FeatureLevels,
		ARRAYSIZE(FeatureLevels),
		D3D11_SDK_VERSION,
		device.GetAddressOf(),
		&m_d3dFeatureLevels,
		context.GetAddressOf());

	if (FAILED(hr))
	{
		return hr;
	}

	// 만들어진 포인터를 맴버 변수에 저장함
	device.As(&m_pd3dDevice);
	context.As(&m_pd3dDeviceContext);

	return hr;
}

HRESULT DXGraphics::CreateSwapChain()
{
	HRESULT hr = S_OK;

	// 스왑 체인 설정
	DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc1{};
	ZeroMemory(&dxgiSwapChainDesc1, sizeof(dxgiSwapChainDesc1));
	dxgiSwapChainDesc1.Width = 0;
	dxgiSwapChainDesc1.Height = 0;
	dxgiSwapChainDesc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	dxgiSwapChainDesc1.Stereo = false;
	dxgiSwapChainDesc1.SampleDesc.Count = 1;
	dxgiSwapChainDesc1.SampleDesc.Quality = 0;
	dxgiSwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc1.BufferCount = 2;
	dxgiSwapChainDesc1.Scaling = DXGI_SCALING_NONE;
	dxgiSwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // 이 경우에 format은 B8G8R8A8, R8G8B8A8로 해ㅑ함
	dxgiSwapChainDesc1.Flags = 0;

	// 임시 포인터 생성
	ComPtr<IDXGIDevice2> device2;
	m_pd3dDevice.As(&device2);

	device2->SetMaximumFrameLatency(1);

	// 디바이스로 어뎁터를 가져옴
	ComPtr<IDXGIAdapter> adapter;
	device2->GetAdapter(&adapter);

	// 어뎁터에서 팩토리를 만듬
	ComPtr<IDXGIFactory2> factory2;
	adapter->GetParent(
		__uuidof(IDXGIFactory2),
		&factory2
	);

	// HWND로 스왑체인 생성
	ComPtr<IDXGISwapChain1> swapChain1;
	hr = factory2->CreateSwapChainForHwnd(
		m_pd3dDevice.Get(),
		m_hWnd,
		&dxgiSwapChainDesc1,
		NULL,
		NULL,
		&swapChain1
	);

	if (FAILED(hr))
	{
		return hr;
	}

	// 만들어진 포인터를 맴버 변수에 저장함
	swapChain1.As(&m_pDXGISwapChain1);

	return hr;
}

HRESULT DXGraphics::CreateRenderTargetView()
{
	HRESULT hr = S_OK;

	// 스왑체인이 가지고 있는 버퍼를 가져옴
	ComPtr<ID3D11Texture2D> backBuffer;
	hr = m_pDXGISwapChain1->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		&backBuffer
	);

	if (FAILED(hr))
	{
		return hr;
	}

	backBuffer.As(&m_backBuffer);

	ComPtr<ID3D11RenderTargetView> rendertarget;
	hr = m_pd3dDevice->CreateRenderTargetView(
		m_backBuffer.Get(),
		nullptr,
		&rendertarget
	);

	if (FAILED(hr))
	{
		return hr;
	}

	rendertarget.As(&m_pd3dRenderTargetView);

	// 만들어진 백버퍼로부터 정보를 받아옴
	ZeroMemory(&m_d3dBackBufferDesc, sizeof(m_d3dBackBufferDesc));
	m_backBuffer->GetDesc(&m_d3dBackBufferDesc);

	ZeroMemory(&m_d3dViewport, sizeof(m_d3dViewport));
	m_d3dViewport.TopLeftX = 0.f;
	m_d3dViewport.TopLeftY = 0.f;
	m_d3dViewport.Width = static_cast<float>(m_d3dBackBufferDesc.Width);
	m_d3dViewport.Height = static_cast<float>(m_d3dBackBufferDesc.Height);
	m_d3dViewport.MinDepth = D3D11_MIN_DEPTH;
	m_d3dViewport.MaxDepth = D3D11_MAX_DEPTH;

	SetCamera();

	return hr;
}

HRESULT DXGraphics::CreateDepthStencilView()
{
	HRESULT hr = S_OK;

	// 백버퍼의 정보를 가져옴
	D3D11_TEXTURE2D_DESC backBufferDesc{};
	ZeroMemory(&backBufferDesc, sizeof(backBufferDesc));
	m_backBuffer->GetDesc(&backBufferDesc);

	// 백버퍼의 정보로
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = backBufferDesc.Width;
	depthStencilDesc.Height = backBufferDesc.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthStencil;
	hr = m_pd3dDevice->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&depthStencil);

	if (FAILED(hr))
	{
		return hr;
	}

	depthStencil.As(&m_pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	ComPtr<ID3D11DepthStencilView> depthStencilView;
	hr = m_pd3dDevice->CreateDepthStencilView(
		m_pDepthStencil.Get(),
		&depthStencilViewDesc,
		&depthStencilView);

	if (FAILED(hr))
	{
		return hr;
	}

	depthStencilView.As(&m_pd3dDepthStencilView);

	return hr;
}

void DXGraphics::SetCamera()
{
	// 월드 행렬
	// 이건 오브젝트마다 가지고 있을것.
	m_world = Matrix4x4::IdentityMatrix();

	Vector3D eye{ 0.0f, 8.f, -9.f };
	Vector3D at{ 0.0f, 6.f, 0.0f };
	Vector3D up{ 0.0f, 1.0f, 0.0f };
	camera.CameraLookAtLH(eye, at, up);

	m_view = camera.GetViewMatrix();

	// 화면 비율
	float aspectRatioX = static_cast<float>(m_d3dBackBufferDesc.Width) / static_cast<float>(m_d3dBackBufferDesc.Height);
	float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

	// 투영 매트릭스
	camera.CameraPerspectiveFovLH(
		2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) / aspectRatioY),
		aspectRatioX,
		0.01f,
		100.0f
	);
	camera.CameraOrthographicLH(4.5f, 3.f, 0.01f, 100.f);

	m_projection = camera.GetProjMatrix();

	m_pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

HRESULT DXGraphics::CreateRasterState()
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC solidRasterDesc;				// 채우는 모드
	ZeroMemory(&solidRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidRasterDesc.FillMode = D3D11_FILL_SOLID;			// 채우기 모드
	solidRasterDesc.CullMode = D3D11_CULL_BACK;			// 지정된 방향은 그리지 않음 (BACK이니 뒷면은 그리지 않음)	
	solidRasterDesc.FrontCounterClockwise = false;		// 시계방향으로 할 거임
	solidRasterDesc.DepthClipEnable = true;				// 거리에 따라 클리핑을 할지

	hr = m_pd3dDevice->CreateRasterizerState(
		&solidRasterDesc,
		m_solidRasterizerState.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_RASTERIZER_DESC wireRasterDesc;						// 채우지 않는 모드
	ZeroMemory(&wireRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireRasterDesc.FillMode = D3D11_FILL_WIREFRAME;			// 채우기 모드
	wireRasterDesc.CullMode = D3D11_CULL_NONE;				// 지정된 방향은 그리지 않음
	wireRasterDesc.FrontCounterClockwise = false;				// 시계방향으로 할 거임
	wireRasterDesc.DepthClipEnable = true;					// 거리에 따라 클리핑을 할지

	hr = m_pd3dDevice->CreateRasterizerState(
		&wireRasterDesc,
		m_wireRasterizerState.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	m_solidRasterizerState.As(&m_currRasterizerState);

	return hr;
}

HRESULT DXGraphics::CreateObject()
{
	HRESULT hr = S_OK;

	// hr = CreateCube();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"큐브 오브젝트 초기화 실패", L"오브젝트 설정 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateAxis();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"축 오브젝트 초기화 실패", L"오브젝트 설정 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateGrid();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"그리드 오브젝트 초기화 실패", L"오브젝트 설정 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	// hr = CreateSkull();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"스컬 오브젝트 초기화 실패", L"오브젝트 설정 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateMeshObject();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"메쉬 오브젝트 초기화 실패", L"오브젝트 설정 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	return hr;
}

HRESULT DXGraphics::CreateCube()
{
	HRESULT hr = S_OK;

	box = new Box(m_pd3dDevice, m_pd3dDeviceContext, m_currRasterizerState);
	if (box == nullptr)
	{
		return S_FALSE;
	}
	hr = box->Initialize();

	return hr;
}

HRESULT DXGraphics::CreateAxis()
{
	HRESULT hr = S_OK;

	axis = new Axis(m_pd3dDevice, m_pd3dDeviceContext, m_wireRasterizerState);
	if (axis == nullptr)
	{
		return S_FALSE;
	}
	hr = axis->Initialize();

	return hr;
}

HRESULT DXGraphics::CreateGrid()
{
	HRESULT hr = S_OK;

	grid = new Grid(m_pd3dDevice, m_pd3dDeviceContext, m_wireRasterizerState);
	if (grid == nullptr)
	{
		return S_FALSE;
	}
	hr = grid->Initialize();

	return hr;
}

HRESULT DXGraphics::CreateSkull()
{
	HRESULT hr = S_OK;

	skull = new Skull(m_pd3dDevice, m_pd3dDeviceContext, m_currRasterizerState);
	if (skull == nullptr)
	{
		return S_FALSE;
	}
	hr = skull->Initialize();

	return hr;
}

HRESULT DXGraphics::CreateMeshObject()
{
	HRESULT hr = S_OK;

	m_parser = new CASEParser();
	m_parser->Init();

	std::wstring ob1 = L"Genji_texture1";
	std::wstring ob2 = L"03IK-Joe-Mesh";
	std::wstring ob3 = L"03IK-Joe";

	object[ob1] = std::make_pair((LPSTR)"../ASEFile/genji_max.ASE", L"../Textures/000000002405_reverse.dds");
	object[ob2] = std::make_pair((LPSTR)"../ASEFile/03IK-Joe_onlymesh.ASE", L"");
	object[ob3] = std::make_pair((LPSTR)"../ASEFile/03IK-Joe.ASE", L"");




	m_parser->Load(object[ob2].first);			// 경로에 있는 파일 열어서 로드
	int index = m_parser->m_MeshList.size();	// 로드된 메쉬들 개수를 받아와서 그만큼 반복함
	for (auto i = 0; i < index; i++)
	{
		ASEParser::Mesh* mesh = m_parser->m_MeshList[i];	// 괜히 복잡해보이지만 일단 조사식 편하게 보려고 꺼냄
		if (mesh->m_type == 3)
		{
			continue;
		}
		MeshObject* newMesh = new MeshObject(m_pd3dDevice, m_pd3dDeviceContext, m_currRasterizerState);
		m_objectList.push_back(newMesh);					// 메쉬를 배열에 넣음
		newMesh->nodeName.assign(mesh->m_nodename.begin(), mesh->m_nodename.end());	// 노드 이름 복사함
		nodeList[newMesh->nodeName] = newMesh;				// 노드이름, 메쉬 포인터를 맵으로 저장해둠
															/// 맵으로 굳이 저장해놓고 배열로 처리한다? 그냥 맵으로 할까..?	
		newMesh->LoadTexture(object[ob2].second);			/// 텍스쳐를 로드함
		newMesh->m_type = mesh->m_type;
		newMesh->LoadGeometry(m_parser->GetOptMesh(i));		/// 버텍스 수를 줄인 다음 버텍스 버퍼를 만듬
		newMesh->LoadNodeData(mesh);						/// 부모 노드 이름을 저장함.
	}

	//
	for (auto& e : m_objectList)					/// 모든 메쉬가 로드된 후에 노드를 연결해줌
	{
		if (nodeList[e->parentName])
		{
			e->parent = nodeList[e->parentName];
		}
	}

	for (auto& e : m_objectList)
	{
		e->InitializeLocalTM();
	}

	/// 연결이 됐으니... 이제는 로컬 스페이스로 되돌려야한다.

	for (auto& e : m_objectList)
	{
		e->Initialize();
	}

	return hr;
}

void DXGraphics::BeginDraw()
{
	// 랜더 타켓 설정.
	m_pd3dDeviceContext->OMSetRenderTargets(
		1,
		m_pd3dRenderTargetView.GetAddressOf(),
		m_pd3dDepthStencilView.Get()
	);

	// 랜더 타겟 뷰 클리어
	m_pd3dDeviceContext->ClearRenderTargetView(
		m_pd3dRenderTargetView.Get(), 
		KHColor::Black
	);

	// 뎁스 스텐실 뷰 클리어
	m_pd3dDeviceContext->ClearDepthStencilView(
		m_pd3dDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
		1.0f, 
		0
	);

	/// 쉐이더 코드 자리?? 인듯??	

	grid->Render();
	axis->Render();
	// box->Render();
	// skull->Render();
	for (auto& e : m_objectList)
	{
		e->Render();
	}
}

void DXGraphics::EndDraw()
{
	m_pDXGISwapChain1->Present(1, 0);
}

 