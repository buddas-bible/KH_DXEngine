#include "DXGraphics.h"

#include <fstream>
#include <vector>

#include "InputManager.h"
#include "TimeController.h"

using namespace Microsoft::WRL;

constexpr float PI = 3.14159265f;

DXGraphics::~DXGraphics()
{
	
}

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

	hr = CreateInputLayout();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"인풋레이아웃 생성 실패", L"초기화 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	// Test();

	return hr;
}

void DXGraphics::Finalize()
{

}

void DXGraphics::Update()
{
	if (GetAsyncKeyState(0x41))
	{
		angleX -= PI / 5 * 0.016f;
		// m_eye.x -= 1.f * 0.016f;
	}
	if (GetAsyncKeyState(0x44))
	{
		m_pos.x -= 1.f * 0.016f;
		// m_eye.x -= 1.f * 0.016f;
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		m_pos.x -= 1.f * 0.016f;
		// m_eye.x -= 1.f * 0.016f;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_pos.x += 1.f * 0.016f;
		// m_eye.x += 1.f * 0.016f;
	}
	if (GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_UP))
	{
		m_pos.y += 1.f * 0.016f;
		// m_eye.y += 1.f * 0.016f;
	}
	if (GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_DOWN))
	{
		m_pos.y -= 1.f * 0.016f;
		// m_eye.y -= 1.f * 0.016f;
	}
	if (GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_SHIFT))
	{
		m_pos.z += 1.f * 0.016f;
		// m_eye.z += 1.f * 0.016f;
	}
	if (GetAsyncKeyState(VK_DOWN) && !GetAsyncKeyState(VK_SHIFT))
	{
		m_pos.z -= 1.f * 0.016f;
		// m_eye.z -= 1.f * 0.016f;
	}


	if (GetAsyncKeyState(VK_F1))
	{
		m_solidRasterizerState.As(&m_currRasterizerState);
	}
	if (GetAsyncKeyState(VK_F2))
	{
		m_wireRasterizerState.As(&m_currRasterizerState);
	}

	if (angleX > 2 * PI)
	{
		angleX -= 2 * PI;
	}
	if (angleX < -2 * PI)
	{
		angleX += 2 * PI;
	}
	if (angleY > 2 * PI)
	{
		angleY -= 2 * PI;
	}
	if (angleY < -2 * PI)
	{
		angleY += 2 * PI;
	}

	/*
	m_constantBufferData.world =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_pos.x, m_pos.y, m_pos.z, 1.f
	};
	*/
	// DirectX::XMVECTOR eye = DirectX::XMVectorSet(m_eye.x, m_eye.y, m_eye.z, m_eye.w);
	// DirectX::XMVECTOR at = DirectX::XMVectorSet(m_eye.x + m_at.x, m_eye.y + m_at.y, m_eye.z + m_at.z, m_eye.w);
	// DirectX::XMVECTOR up = DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, m_up.w);

	// 원래는 카메라 포지션에 카메라 기저에 내적한 것에 -를 붙여줘야 하지만
	// 뷰 행렬에 임의로 넣어주기로 하자.
	// m_constantBufferData.view.r[0].m128_f32[0] = -m_pos.x;
	// m_constantBufferData.view.r[0].m128_f32[1] = -m_pos.y;
	// m_constantBufferData.view.r[0].m128_f32[2] = -m_pos.z;
	// m_constantBufferData.view.r[0].m128_f32[3] = 0.f;
	// 
	// m_constantBufferData.view.r[1].m128_f32[0] = -m_pos.x;
	// m_constantBufferData.view.r[1].m128_f32[1] = -m_pos.y;
	// m_constantBufferData.view.r[1].m128_f32[2] = -m_pos.z;
	// m_constantBufferData.view.r[1].m128_f32[3] = 0.f;
	// 
	// m_constantBufferData.view.r[2].m128_f32[0] = -m_pos.x;
	// m_constantBufferData.view.r[2].m128_f32[1] = -m_pos.y;
	// m_constantBufferData.view.r[2].m128_f32[2] = -m_pos.z;
	// m_constantBufferData.view.r[2].m128_f32[3] = 0.f;

	m_constantBufferData.view.r[3].m128_f32[0] = -m_pos.x;
	m_constantBufferData.view.r[3].m128_f32[1] = -m_pos.y;
	m_constantBufferData.view.r[3].m128_f32[2] = -m_pos.z;
	m_constantBufferData.view.r[3].m128_f32[3] = 1.f;
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

	m_pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);

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

	return hr;
}

HRESULT DXGraphics::CreateObject()
{
	HRESULT hr = S_OK;

	hr = CreateCube();
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

	hr = CreateCubeShaders();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"셰이더 초기화 실패", L"오브젝트 설정 오류", MB_OK | MB_ICONWARNING);
		return hr;
	}

	return hr;
}

HRESULT DXGraphics::CreateInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC textureDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// 인풋 레이아웃 만듬
	D3DX11_PASS_DESC cubePassDesc;
	m_cubeTechnique->GetPassByIndex(0)->GetDesc(&cubePassDesc);
	hr = m_pd3dDevice->CreateInputLayout(
		textureDesc,
		ARRAYSIZE(textureDesc),						// 버텍스에 들어간 데이터 갯수
		cubePassDesc.pIAInputSignature,				// 셰이더 코드 포인터
		cubePassDesc.IAInputSignatureSize,			// 셰이더 크기
		m_cubeInputLayout.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	// 버텍스 정보가 어떻게 되어있는지 설정
	D3D11_INPUT_ELEMENT_DESC iaDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	D3DX11_PASS_DESC passDesc;
	m_axisTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = m_pd3dDevice->CreateInputLayout(
		iaDesc,
		ARRAYSIZE(iaDesc),						// 버텍스에 들어간 데이터 갯수
		passDesc.pIAInputSignature,				// 셰이더 코드 포인터
		passDesc.IAInputSignatureSize,			// 셰이더 크기
		m_axisInputLayout.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}
	
	m_gridTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = m_pd3dDevice->CreateInputLayout(
		iaDesc,
		ARRAYSIZE(iaDesc),						// 버텍스에 들어간 데이터 갯수
		passDesc.pIAInputSignature,				// 셰이더 코드 포인터
		passDesc.IAInputSignatureSize,			// 셰이더 크기
		m_gridInputLayout.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}
	

	return hr;
}

/// <summary>
/// 초기화 할 때 쓰던 함수
/// </summary>
/*
void DXGraphics::Test()
{
	{
		m_pd3dDeviceContext->OMSetRenderTargets(
			1,
			m_pd3dRenderTargetView.GetAddressOf(),
			m_pd3dDepthStencilView.Get());

		const float clearColor[] = { 0.561f, 0.04f, 0.071f, 1.0f };
		m_pd3dDeviceContext->ClearRenderTargetView(
			m_pd3dRenderTargetView.Get(),
			clearColor);

		m_pDXGISwapChain1->Present(1, 0);

		m_pd3dDeviceContext->OMSetRenderTargets(
			1,
			m_pd3dRenderTargetView.GetAddressOf(),
			m_pd3dDepthStencilView.Get());

		const float clearColor1[] = { 0.071f, 0.04f, 0.561f, 1.0f };
		m_pd3dDeviceContext->ClearRenderTargetView(
			m_pd3dRenderTargetView.Get(),
			clearColor1);

		m_pDXGISwapChain1->Present(1, 0);
	}
}
*/ 

HRESULT DXGraphics::CreateCubeShaders()
{
	HRESULT hr = S_OK;

#if _WIN64

#if _DEBUG
	std::ifstream cfin("../x64/debug/Texture.cso", std::ios::binary);
#else
	std::ifstream cfin("../x64/release/Texture.cso", std::ios::binary);		// 이부분 경로 다시 생각해보자...
#endif

#else

#if _DEBUG
	std::ifstream cfin("../WIN32/debug/Texture.cso", std::ios::binary);
#else
	std::ifstream cfin("../WIN32/release/Texture.cso", std::ios::binary);
#endif

#endif

	cfin.seekg(0, std::ios_base::end);
	int size = (int)cfin.tellg();
	cfin.seekg(0, std::ios_base::beg);
	std::vector<char> cCompiledShader(size);

	cfin.read(&cCompiledShader[0], size);
	cfin.close();

	/*
	ComPtr<ID3D11VertexShader> vertexShader;
	hr = m_pd3dDevice->CreateVertexShader(
		&vsCompiledShader[0],
		size,
		0,
		vertexShader.GetAddressOf()
	);
	*/

	/*
	UINT flags = 0;
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3D10_SHADER_SKIP_OPTIMIZATION;

	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;
	hr = D3DCompileFromFile(
		L"./VertexShader.fx",					// 셰이더 파일 이름
		0,										// 셰이더 매크로
		0,										//
		0,										// 진입점
		"fx_5_0",								// 셰이더 기능
		flags,
		0,
		&shaderBlob,
		&errorBlob
	);

	if (FAILED(hr))
	{
		return hr;
	}
	*/

	/// 파일로부터 이펙트 정보를 읽어옴
	// 큐브 이펙트
	{
		hr = D3DX11CreateEffectFromMemory(
			&cCompiledShader[0],
			size,
			0,
			m_pd3dDevice.Get(),
			m_cubeEffect.GetAddressOf()
		);

		m_cubeTechnique = m_cubeEffect->GetTechniqueByName("Tech");								// 파일에 Tech 이름의 데이터를 읽어옴
		m_cubeMatrixVariable = m_cubeEffect->GetVariableByName("worldViewProj")->AsMatrix();	// 파일에 worldViewProj 이름의 데이터를 읽어옴
		
		// 이펙트로 셰이더 리소스랑 샘플러가 필요하지 않을까?
		m_cubeShaderResource = m_cubeEffect->GetVariableByName("g_Texture")->AsShaderResource();	// 
		m_cubeSampler = m_cubeEffect->GetVariableByName("g_Sampler")->AsSampler();
	}

	if (FAILED(hr))
	{
		return hr;
	}

#if _WIN64

#if _DEBUG
	std::ifstream fin("../x64/debug/Color.cso", std::ios::binary);
#else
	std::ifstream fin("../x64/release/Color.cso", std::ios::binary);
#endif

#else

#if _DEBUG
	std::ifstream fin("../WIN32/debug/Color.cso", std::ios::binary);
#else
	std::ifstream fin("../WIN32/release/Color.cso", std::ios::binary);
#endif

#endif

	fin.seekg(0, std::ios_base::end);
	int cSize = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> vsCompiledShader(cSize);

	fin.read(&vsCompiledShader[0], cSize);
	fin.close();

	// 축 이펙트
	{
		hr = D3DX11CreateEffectFromMemory(
			&vsCompiledShader[0],
			cSize,
			0,
			m_pd3dDevice.Get(),
			m_axisEffect.GetAddressOf()
		);

		m_axisTechnique = m_axisEffect->GetTechniqueByName("Tech");							// 파일에 Tech 이름의 데이터를 읽어옴
		m_axisMatrixVariable = m_axisEffect->GetVariableByName("worldViewProj")->AsMatrix();	// 파일에 worldViewProj 이름의 데이터를 읽어옴
	}

	if (FAILED(hr))
	{
		return hr;
	}

	// 그리드 이펙트
	{
		hr = D3DX11CreateEffectFromMemory(
			&vsCompiledShader[0],
			size,
			0,
			m_pd3dDevice.Get(),
			m_gridEffect.GetAddressOf()
		);

		m_gridTechnique = m_gridEffect->GetTechniqueByName("Tech");							// 파일에 Tech 이름의 데이터를 읽어옴
		m_gridMatrixVariable = m_gridEffect->GetVariableByName("worldViewProj")->AsMatrix();	// 파일에 worldViewProj 이름의 데이터를 읽어옴
	}
	
	if (FAILED(hr))
	{
		return hr;
	}

	// vertexShader.As(&m_vertexShader);
	/*
	Vector4D _eye(0.0f, 0.7f, -1.5f, 1.f);
	Vector4D _at(0.0f, 0.1f, 0.0f, 1.f);
	Vector4D _up(0.0f, 0.7f, -1.5f, 1.f);

	Vector4D z = _at = _eye;
	Vector4D x = _up.Cross(z);
	Vector4D y = z.Cross(y);

	z = z.Normalize();
	y = y.Normalize();
	x = x.Normalize();

	m_x = { x.x, x.y, x.z, x.w };
	m_y = { y.x, y.y, y.z, y.w };
	m_z = { z.x, z.y, z.z, z.w };
	*/

	/// 지금은 상수로 매트릭스를 구성했지만
	/// 꾸준히 업데이트 되는 요소로 봐야할 것
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, -1.5f, 1.f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.1f, 0.0f, 1.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.f);

	m_pos = { 0.0f, 0.7f, -1.5f, 1.f };

	// 월드 행렬
	m_constantBufferData.world = DirectX::XMMatrixIdentity();

	// 뷰 매트릭스
	m_constantBufferData.view = 
		DirectX::XMMatrixLookAtLH(
			eye,							// 카메라 위치
			at,								// 카메라 초점
			up								// 카메라 업벡터
		);

	// 백버퍼에서 정보를 가져옴
	D3D11_TEXTURE2D_DESC backBufferDesc{};
	ZeroMemory(&backBufferDesc, sizeof(backBufferDesc));
	m_backBuffer->GetDesc(&backBufferDesc);

	// 화면 비율
	float aspectRatioX = static_cast<float>(backBufferDesc.Width) / static_cast<float>(backBufferDesc.Height);
	float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

	// 투영 매트릭스
	m_constantBufferData.projection =
			DirectX::XMMatrixPerspectiveFovLH(
				2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) / aspectRatioY),
				aspectRatioX,
				0.01f,
				100.0f
		);

	/*
	std::ifstream psfin("CubePixelShader.cso", std::ios::binary);

	psfin.seekg(0, std::ios_base::end);
	int pssize = (int)psfin.tellg();
	psfin.seekg(0, std::ios_base::beg);
	std::vector<char> psCompiledShader(pssize);

	psfin.read(&psCompiledShader[0], pssize);
	psfin.close();

	ComPtr<ID3D11PixelShader> pixelShader;
	hr = m_pd3dDevice->CreatePixelShader(
		&psCompiledShader[0], 
		pssize,
		0, 
		pixelShader.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	pixelShader.As(&m_pixelShader);

	CD3D11_BUFFER_DESC constantBufferDesc(
		sizeof(ConstantBuffer),
		D3D11_BIND_CONSTANT_BUFFER
	);

	hr = m_pd3dDevice->CreateBuffer(
		&constantBufferDesc, 
		nullptr, 
		m_constantBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	/// 상수 버퍼 설정하기 위한 것
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

	DirectX::XMStoreFloat4x4(
		&m_constantBufferData.view,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookAtRH(
				eye,
				at,
				up
			)
		)
	);

	D3D11_TEXTURE2D_DESC backBufferDesc{};
	ZeroMemory(&backBufferDesc, sizeof(backBufferDesc));
	m_backBuffer->GetDesc(&backBufferDesc);

	float aspectRatioX = static_cast<float>(backBufferDesc.Width) / static_cast<float>(backBufferDesc.Height);
	float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

	DirectX::XMStoreFloat4x4(
		&m_constantBufferData.projection,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixPerspectiveFovRH(
				2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) / aspectRatioY),
				aspectRatioX,
				0.01f,
				100.0f
			)
		)
	);
	*/

	return hr;
}

HRESULT DXGraphics::CreateCube()
{
	HRESULT hr = S_OK;

	// 꼭짓점을 설명하는 정보
	TextureVertex cube[] =
	{
		/*
		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f),	 DirectX::XMFLOAT4(Color::Black),},
		{DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f),  DirectX::XMFLOAT4(Color::Red),},
		{DirectX::XMFLOAT3(0.5f,-0.5f, 0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f),	 DirectX::XMFLOAT4(Color::Magenta),},
		{DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.5f, 1.f),  DirectX::XMFLOAT4(Color::Blue),},

		{DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f),  DirectX::XMFLOAT4(Color::Green),},
		{DirectX::XMFLOAT3(0.5f, 0.5f,-0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f),	 DirectX::XMFLOAT4(Color::Yellow),},
		{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.7f, 0.7f), DirectX::XMFLOAT4(Color::White),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f),	 DirectX::XMFLOAT4(Color::Cyan),},
		*/
		{ DirectX::XMFLOAT3(-0.520968f,0.592536f,-0.288356f), DirectX::XMFLOAT2(0.625,0.375), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.233041,0.355878,-0.79007), DirectX::XMFLOAT2(0.625,0.53125), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.468286,0.703697,-0.310557), DirectX::XMFLOAT2(0.65625,0.375), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.233041,0.355878,-0.79007), DirectX::XMFLOAT2(0.625,0.53125), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		/*
		{ DirectX::XMFLOAT3(-0.18036,0.467039,-0.812271), DirectX::XMFLOAT2(0.65625,0.53125), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.468286,0.703697,-0.310557), DirectX::XMFLOAT2(0.65625,0.375), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.614746,0.75179,-0.417294), DirectX::XMFLOAT2(0.421875,0.625), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.32682,0.515133,-0.919008), DirectX::XMFLOAT2(0.421875,0.78125), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.667428,0.640629,-0.395093), DirectX::XMFLOAT2(0.453125,0.625), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.32682,0.515133,-0.919008), DirectX::XMFLOAT2(0.421875,0.78125), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.379501,0.403972,-0.896807), DirectX::XMFLOAT2(0.453125,0.78125), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.667428,0.640629,-0.395093), DirectX::XMFLOAT2(0.453125,0.625), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.614746,0.75179,-0.417294), DirectX::XMFLOAT2(0.828125,0.1875), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.667428,0.640629,-0.395093), DirectX::XMFLOAT2(0.828125,0.15625), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.468286,0.703697,-0.310557), DirectX::XMFLOAT2(0.78125,0.1875), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.667428,0.640629,-0.395093), DirectX::XMFLOAT2(0.828125,0.15625), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.520968,0.592536,-0.288356), DirectX::XMFLOAT2(0.78125,0.15625), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },{ DirectX::XMFLOAT3(-0.468286,0.703697,-0.310557), DirectX::XMFLOAT2(0.78125,0.1875), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.379501,0.403972,-0.896807), DirectX::XMFLOAT2(0.828125,0.1875), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },{ DirectX::XMFLOAT3(-0.32682,0.515133,-0.919008), DirectX::XMFLOAT2(0.828125,0.21875), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },{ DirectX::XMFLOAT3(-0.233041,0.355878,-0.79007), DirectX::XMFLOAT2(0.78125,0.1875), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },
		{ DirectX::XMFLOAT3(-0.32682,0.515133,-0.919008), DirectX::XMFLOAT2(0.828125,0.21875), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },{ DirectX::XMFLOAT3(-0.18036,0.467039,-0.812271), DirectX::XMFLOAT2(0.78125,0.21875), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },{ DirectX::XMFLOAT3(-0.233041,0.355878,-0.79007), DirectX::XMFLOAT2(0.78125,0.1875), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },
		{ DirectX::XMFLOAT3(-0.667428,0.640629,-0.395093), DirectX::XMFLOAT2(0.5625,0.25), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },{ DirectX::XMFLOAT3(-0.379501,0.403972,-0.896807), DirectX::XMFLOAT2(0.5625,0.40625), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },{ DirectX::XMFLOAT3(-0.520968,0.592536,-0.288356), DirectX::XMFLOAT2(0.609375,0.25), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },
		{ DirectX::XMFLOAT3(-0.379501,0.403972,-0.896807), DirectX::XMFLOAT2(0.5625,0.40625), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },{ DirectX::XMFLOAT3(-0.233041,0.355878,-0.79007), DirectX::XMFLOAT2(0.609375,0.40625), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },{ DirectX::XMFLOAT3(-0.520968,0.592536,-0.288356), DirectX::XMFLOAT2(0.609375,0.25), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },
		{ DirectX::XMFLOAT3(-0.468286,0.703697,-0.310557), DirectX::XMFLOAT2(0.203125,0.5625), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },{ DirectX::XMFLOAT3(-0.18036,0.467039,-0.812271), DirectX::XMFLOAT2(0.203125,0.71875), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },{ DirectX::XMFLOAT3(-0.614746,0.75179,-0.417294), DirectX::XMFLOAT2(0.25,0.5625), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },
		{ DirectX::XMFLOAT3(-0.18036,0.467039,-0.812271), DirectX::XMFLOAT2(0.203125,0.71875), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },{ DirectX::XMFLOAT3(-0.32682,0.515133,-0.919008), DirectX::XMFLOAT2(0.25,0.71875), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },{ DirectX::XMFLOAT3(-0.614746,0.75179,-0.417294), DirectX::XMFLOAT2(0.25,0.5625), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },
		{ DirectX::XMFLOAT3(0.330002,1.410313,-0.299831), DirectX::XMFLOAT2(0.6875,0.40625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.315445,1.057489,-0.173622), DirectX::XMFLOAT2(0.6875,0.5), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.336304,1.45213,-0.182202), DirectX::XMFLOAT2(0.71875,0.40625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.315445,1.057489,-0.173622), DirectX::XMFLOAT2(0.6875,0.5), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.321748,1.099307,-0.055993), DirectX::XMFLOAT2(0.71875,0.5), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.336304,1.45213,-0.182202), DirectX::XMFLOAT2(0.71875,0.40625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.523425,1.442099,-0.188662), DirectX::XMFLOAT2(0.65625,0.703125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.508868,1.089276,-0.062453), DirectX::XMFLOAT2(0.65625,0.796875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.517122,1.400282,-0.306291), DirectX::XMFLOAT2(0.6875,0.703125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.508868,1.089276,-0.062453), DirectX::XMFLOAT2(0.65625,0.796875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.502566,1.047459,-0.180082), DirectX::XMFLOAT2(0.6875,0.796875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.517122,1.400282,-0.306291), DirectX::XMFLOAT2(0.6875,0.703125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.523425,1.442099,-0.188662), DirectX::XMFLOAT2(0.421875,0.796875), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.517122,1.400282,-0.306291), DirectX::XMFLOAT2(0.421875,0.765625), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.336304,1.45213,-0.182202), DirectX::XMFLOAT2(0.375,0.796875), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },
		{ DirectX::XMFLOAT3(0.517122,1.400282,-0.306291), DirectX::XMFLOAT2(0.421875,0.765625), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.330002,1.410313,-0.299831), DirectX::XMFLOAT2(0.375,0.765625), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.336304,1.45213,-0.182202), DirectX::XMFLOAT2(0.375,0.796875), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },
		{ DirectX::XMFLOAT3(0.502566,1.047459,-0.180082), DirectX::XMFLOAT2(0.734375,0.765625), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.508868,1.089276,-0.062453), DirectX::XMFLOAT2(0.734375,0.796875), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.315445,1.057489,-0.173622), DirectX::XMFLOAT2(0.6875,0.765625), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },
		{ DirectX::XMFLOAT3(0.508868,1.089276,-0.062453), DirectX::XMFLOAT2(0.734375,0.796875), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.321748,1.099307,-0.055993), DirectX::XMFLOAT2(0.6875,0.796875), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.315445,1.057489,-0.173622), DirectX::XMFLOAT2(0.6875,0.765625), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },
		{ DirectX::XMFLOAT3(0.517122,1.400282,-0.306291), DirectX::XMFLOAT2(0.5,0.625), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.502566,1.047459,-0.180082), DirectX::XMFLOAT2(0.5,0.71875), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.330002,1.410313,-0.299831), DirectX::XMFLOAT2(0.546875,0.625), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.502566,1.047459,-0.180082), DirectX::XMFLOAT2(0.5,0.71875), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.315445,1.057489,-0.173622), DirectX::XMFLOAT2(0.546875,0.71875), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.330002,1.410313,-0.299831), DirectX::XMFLOAT2(0.546875,0.625), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.336304,1.45213,-0.182202), DirectX::XMFLOAT2(0.453125,0.625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.321748,1.099307,-0.055993), DirectX::XMFLOAT2(0.453125,0.71875), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.523425,1.442099,-0.188662), DirectX::XMFLOAT2(0.5,0.625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.321748,1.099307,-0.055993), DirectX::XMFLOAT2(0.453125,0.71875), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.508868,1.089276,-0.062453), DirectX::XMFLOAT2(0.5,0.71875), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.523425,1.442099,-0.188662), DirectX::XMFLOAT2(0.5,0.625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.336867,1.308856,-0.396964), DirectX::XMFLOAT2(0.6875,0.09375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.327162,1.07364,-0.312824), DirectX::XMFLOAT2(0.6875,0.15625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.346321,1.371582,-0.22052), DirectX::XMFLOAT2(0.734375,0.09375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.327162,1.07364,-0.312824), DirectX::XMFLOAT2(0.6875,0.15625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.336616,1.136366,-0.136381), DirectX::XMFLOAT2(0.734375,0.15625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.346321,1.371582,-0.22052), DirectX::XMFLOAT2(0.734375,0.09375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.408694,1.368238,-0.222673), DirectX::XMFLOAT2(0.6875,0.703125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.398989,1.133022,-0.138534), DirectX::XMFLOAT2(0.6875,0.765625), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.39924,1.305512,-0.399117), DirectX::XMFLOAT2(0.734375,0.703125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.398989,1.133022,-0.138534), DirectX::XMFLOAT2(0.6875,0.765625), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.389536,1.070296,-0.314977), DirectX::XMFLOAT2(0.734375,0.765625), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.39924,1.305512,-0.399117), DirectX::XMFLOAT2(0.734375,0.703125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.408694,1.368238,-0.222673), DirectX::XMFLOAT2(0.140625,0.859375), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.39924,1.305512,-0.399117), DirectX::XMFLOAT2(0.140625,0.8125), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.346321,1.371582,-0.22052), DirectX::XMFLOAT2(0.125,0.859375), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },
		{ DirectX::XMFLOAT3(0.39924,1.305512,-0.399117), DirectX::XMFLOAT2(0.140625,0.8125), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.336867,1.308856,-0.396964), DirectX::XMFLOAT2(0.125,0.8125), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.346321,1.371582,-0.22052), DirectX::XMFLOAT2(0.125,0.859375), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },
		{ DirectX::XMFLOAT3(0.389536,1.070296,-0.314977), DirectX::XMFLOAT2(0.15625,0.8125), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.398989,1.133022,-0.138534), DirectX::XMFLOAT2(0.15625,0.859375), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.327162,1.07364,-0.312824), DirectX::XMFLOAT2(0.140625,0.8125), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },
		{ DirectX::XMFLOAT3(0.398989,1.133022,-0.138534), DirectX::XMFLOAT2(0.15625,0.859375), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.336616,1.136366,-0.136381), DirectX::XMFLOAT2(0.140625,0.859375), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.327162,1.07364,-0.312824), DirectX::XMFLOAT2(0.140625,0.8125), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },
		{ DirectX::XMFLOAT3(0.39924,1.305512,-0.399117), DirectX::XMFLOAT2(0.75,0.78125), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.389536,1.070296,-0.314977), DirectX::XMFLOAT2(0.75,0.84375), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.336867,1.308856,-0.396964), DirectX::XMFLOAT2(0.765625,0.78125), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.389536,1.070296,-0.314977), DirectX::XMFLOAT2(0.75,0.84375), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.327162,1.07364,-0.312824), DirectX::XMFLOAT2(0.765625,0.84375), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.336867,1.308856,-0.396964), DirectX::XMFLOAT2(0.765625,0.78125), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.346321,1.371582,-0.22052), DirectX::XMFLOAT2(0.734375,0.78125), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.336616,1.136366,-0.136381), DirectX::XMFLOAT2(0.734375,0.84375), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.408694,1.368238,-0.222673), DirectX::XMFLOAT2(0.75,0.78125), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.336616,1.136366,-0.136381), DirectX::XMFLOAT2(0.734375,0.84375), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.398989,1.133022,-0.138534), DirectX::XMFLOAT2(0.75,0.84375), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.408694,1.368238,-0.222673), DirectX::XMFLOAT2(0.75,0.78125), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.430427,1.30384,-0.400194), DirectX::XMFLOAT2(0.0625,0.71875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.420722,1.068624,-0.316054), DirectX::XMFLOAT2(0.0625,0.78125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.439881,1.366566,-0.22375), DirectX::XMFLOAT2(0.109375,0.71875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.420722,1.068624,-0.316054), DirectX::XMFLOAT2(0.0625,0.78125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.430176,1.131351,-0.139611), DirectX::XMFLOAT2(0.109375,0.78125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.439881,1.366566,-0.22375), DirectX::XMFLOAT2(0.109375,0.71875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.502254,1.363223,-0.225903), DirectX::XMFLOAT2(0.109375,0.71875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.492549,1.128007,-0.141764), DirectX::XMFLOAT2(0.109375,0.78125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.4928,1.300497,-0.402347), DirectX::XMFLOAT2(0.15625,0.71875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.492549,1.128007,-0.141764), DirectX::XMFLOAT2(0.109375,0.78125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.483096,1.065281,-0.318207), DirectX::XMFLOAT2(0.15625,0.78125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.4928,1.300497,-0.402347), DirectX::XMFLOAT2(0.15625,0.71875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.502254,1.363223,-0.225903), DirectX::XMFLOAT2(0.171875,0.859375), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.4928,1.300497,-0.402347), DirectX::XMFLOAT2(0.171875,0.8125), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.439881,1.366566,-0.22375), DirectX::XMFLOAT2(0.15625,0.859375), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },
		{ DirectX::XMFLOAT3(0.4928,1.300497,-0.402347), DirectX::XMFLOAT2(0.171875,0.8125), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.430427,1.30384,-0.400194), DirectX::XMFLOAT2(0.15625,0.8125), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },{ DirectX::XMFLOAT3(0.439881,1.366566,-0.22375), DirectX::XMFLOAT2(0.15625,0.859375), DirectX::XMFLOAT3(0.0388,0.9409,-0.3366) },
		{ DirectX::XMFLOAT3(0.483096,1.065281,-0.318207), DirectX::XMFLOAT2(0.1875,0.8125), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.492549,1.128007,-0.141764), DirectX::XMFLOAT2(0.1875,0.859375), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.420722,1.068624,-0.316054), DirectX::XMFLOAT2(0.171875,0.8125), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },
		{ DirectX::XMFLOAT3(0.492549,1.128007,-0.141764), DirectX::XMFLOAT2(0.1875,0.859375), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.430176,1.131351,-0.139611), DirectX::XMFLOAT2(0.171875,0.859375), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },{ DirectX::XMFLOAT3(0.420722,1.068624,-0.316054), DirectX::XMFLOAT2(0.171875,0.8125), DirectX::XMFLOAT3(-0.0388,-0.9409,0.3366) },
		{ DirectX::XMFLOAT3(0.4928,1.300497,-0.402347), DirectX::XMFLOAT2(0.78125,0.78125), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.483096,1.065281,-0.318207), DirectX::XMFLOAT2(0.78125,0.84375), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.430427,1.30384,-0.400194), DirectX::XMFLOAT2(0.796875,0.78125), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.483096,1.065281,-0.318207), DirectX::XMFLOAT2(0.78125,0.84375), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.420722,1.068624,-0.316054), DirectX::XMFLOAT2(0.796875,0.84375), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.430427,1.30384,-0.400194), DirectX::XMFLOAT2(0.796875,0.78125), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.439881,1.366566,-0.22375), DirectX::XMFLOAT2(0.765625,0.78125), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.430176,1.131351,-0.139611), DirectX::XMFLOAT2(0.765625,0.84375), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.502254,1.363223,-0.225903), DirectX::XMFLOAT2(0.78125,0.78125), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.430176,1.131351,-0.139611), DirectX::XMFLOAT2(0.765625,0.84375), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.492549,1.128007,-0.141764), DirectX::XMFLOAT2(0.78125,0.84375), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.502254,1.363223,-0.225903), DirectX::XMFLOAT2(0.78125,0.78125), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.510569,1.330686,-0.24799), DirectX::XMFLOAT2(0.625,0.578125), DirectX::XMFLOAT3(-0.3473,0.8893,-0.2975) },{ DirectX::XMFLOAT3(0.452045,1.311197,-0.237926), DirectX::XMFLOAT2(0.625,0.59375), DirectX::XMFLOAT3(-0.3473,0.8893,-0.2975) },{ DirectX::XMFLOAT3(0.513721,1.351594,-0.189176), DirectX::XMFLOAT2(0.640625,0.578125), DirectX::XMFLOAT3(-0.3473,0.8893,-0.2975) },
		{ DirectX::XMFLOAT3(0.452045,1.311197,-0.237926), DirectX::XMFLOAT2(0.625,0.59375), DirectX::XMFLOAT3(-0.3473,0.8893,-0.2975) },{ DirectX::XMFLOAT3(0.455196,1.332106,-0.179112), DirectX::XMFLOAT2(0.640625,0.59375), DirectX::XMFLOAT3(-0.3473,0.8893,-0.2975) },{ DirectX::XMFLOAT3(0.513721,1.351594,-0.189176), DirectX::XMFLOAT2(0.640625,0.578125), DirectX::XMFLOAT3(-0.3473,0.8893,-0.2975) },
		{ DirectX::XMFLOAT3(0.600547,1.12927,-0.114791), DirectX::XMFLOAT2(0.640625,0.296875), DirectX::XMFLOAT3(0.3473,-0.8893,0.2975) },{ DirectX::XMFLOAT3(0.542022,1.109781,-0.104727), DirectX::XMFLOAT2(0.640625,0.3125), DirectX::XMFLOAT3(0.3473,-0.8893,0.2975) },{ DirectX::XMFLOAT3(0.597395,1.108361,-0.173605), DirectX::XMFLOAT2(0.65625,0.296875), DirectX::XMFLOAT3(0.3473,-0.8893,0.2975) },
		{ DirectX::XMFLOAT3(0.542022,1.109781,-0.104727), DirectX::XMFLOAT2(0.640625,0.3125), DirectX::XMFLOAT3(0.3473,-0.8893,0.2975) },{ DirectX::XMFLOAT3(0.538871,1.088872,-0.163541), DirectX::XMFLOAT2(0.65625,0.3125), DirectX::XMFLOAT3(0.3473,-0.8893,0.2975) },{ DirectX::XMFLOAT3(0.597395,1.108361,-0.173605), DirectX::XMFLOAT2(0.65625,0.296875), DirectX::XMFLOAT3(0.3473,-0.8893,0.2975) },
		{ DirectX::XMFLOAT3(0.600547,1.12927,-0.114791), DirectX::XMFLOAT2(0.84375,0.3125), DirectX::XMFLOAT3(0.9364,0.3118,-0.161) },{ DirectX::XMFLOAT3(0.597395,1.108361,-0.173605), DirectX::XMFLOAT2(0.84375,0.296875), DirectX::XMFLOAT3(0.9364,0.3118,-0.161) },{ DirectX::XMFLOAT3(0.513721,1.351594,-0.189176), DirectX::XMFLOAT2(0.78125,0.3125), DirectX::XMFLOAT3(0.9364,0.3118,-0.161) },
		{ DirectX::XMFLOAT3(0.597395,1.108361,-0.173605), DirectX::XMFLOAT2(0.84375,0.296875), DirectX::XMFLOAT3(0.9364,0.3118,-0.161) },{ DirectX::XMFLOAT3(0.510569,1.330686,-0.24799), DirectX::XMFLOAT2(0.78125,0.296875), DirectX::XMFLOAT3(0.9364,0.3118,-0.161) },{ DirectX::XMFLOAT3(0.513721,1.351594,-0.189176), DirectX::XMFLOAT2(0.78125,0.3125), DirectX::XMFLOAT3(0.9364,0.3118,-0.161) },
		{ DirectX::XMFLOAT3(0.538871,1.088872,-0.163541), DirectX::XMFLOAT2(0.84375,0.3125), DirectX::XMFLOAT3(-0.9364,-0.3118,0.161) },{ DirectX::XMFLOAT3(0.542022,1.109781,-0.104727), DirectX::XMFLOAT2(0.84375,0.328125), DirectX::XMFLOAT3(-0.9364,-0.3118,0.161) },{ DirectX::XMFLOAT3(0.452045,1.311197,-0.237926), DirectX::XMFLOAT2(0.78125,0.3125), DirectX::XMFLOAT3(-0.9364,-0.3118,0.161) },
		{ DirectX::XMFLOAT3(0.542022,1.109781,-0.104727), DirectX::XMFLOAT2(0.84375,0.328125), DirectX::XMFLOAT3(-0.9364,-0.3118,0.161) },{ DirectX::XMFLOAT3(0.455196,1.332106,-0.179112), DirectX::XMFLOAT2(0.78125,0.328125), DirectX::XMFLOAT3(-0.9364,-0.3118,0.161) },{ DirectX::XMFLOAT3(0.452045,1.311197,-0.237926), DirectX::XMFLOAT2(0.78125,0.3125), DirectX::XMFLOAT3(-0.9364,-0.3118,0.161) },
		{ DirectX::XMFLOAT3(0.597395,1.108361,-0.173605), DirectX::XMFLOAT2(0.75,0.546875), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.538871,1.088872,-0.163541), DirectX::XMFLOAT2(0.75,0.5625), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.510569,1.330686,-0.24799), DirectX::XMFLOAT2(0.8125,0.546875), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.538871,1.088872,-0.163541), DirectX::XMFLOAT2(0.75,0.5625), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.452045,1.311197,-0.237926), DirectX::XMFLOAT2(0.8125,0.5625), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.510569,1.330686,-0.24799), DirectX::XMFLOAT2(0.8125,0.546875), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.513721,1.351594,-0.189176), DirectX::XMFLOAT2(0.65625,0.515625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.455196,1.332106,-0.179112), DirectX::XMFLOAT2(0.65625,0.53125), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.600547,1.12927,-0.114791), DirectX::XMFLOAT2(0.71875,0.515625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.455196,1.332106,-0.179112), DirectX::XMFLOAT2(0.65625,0.53125), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.542022,1.109781,-0.104727), DirectX::XMFLOAT2(0.71875,0.53125), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.600547,1.12927,-0.114791), DirectX::XMFLOAT2(0.71875,0.515625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.602739,1.191822,-0.150706), DirectX::XMFLOAT2(0.640625,0.578125), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },{ DirectX::XMFLOAT3(0.613138,1.13023,-0.152825), DirectX::XMFLOAT2(0.640625,0.59375), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },{ DirectX::XMFLOAT3(0.551356,1.18197,-0.116515), DirectX::XMFLOAT2(0.65625,0.578125), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },
		{ DirectX::XMFLOAT3(0.613138,1.13023,-0.152825), DirectX::XMFLOAT2(0.640625,0.59375), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },{ DirectX::XMFLOAT3(0.561755,1.120378,-0.118635), DirectX::XMFLOAT2(0.65625,0.59375), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },{ DirectX::XMFLOAT3(0.551356,1.18197,-0.116515), DirectX::XMFLOAT2(0.65625,0.578125), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },
		{ DirectX::XMFLOAT3(0.687467,1.197755,0.092589), DirectX::XMFLOAT2(0.65625,0.296875), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },{ DirectX::XMFLOAT3(0.697866,1.136163,0.090469), DirectX::XMFLOAT2(0.65625,0.3125), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },{ DirectX::XMFLOAT3(0.73885,1.207607,0.058399), DirectX::XMFLOAT2(0.671875,0.296875), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },
		{ DirectX::XMFLOAT3(0.697866,1.136163,0.090469), DirectX::XMFLOAT2(0.65625,0.3125), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },{ DirectX::XMFLOAT3(0.749249,1.146015,0.056279), DirectX::XMFLOAT2(0.671875,0.3125), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },{ DirectX::XMFLOAT3(0.73885,1.207607,0.058399), DirectX::XMFLOAT2(0.671875,0.296875), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },
		{ DirectX::XMFLOAT3(0.687467,1.197755,0.092589), DirectX::XMFLOAT2(0.84375,0.375), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },{ DirectX::XMFLOAT3(0.73885,1.207607,0.058399), DirectX::XMFLOAT2(0.84375,0.359375), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },{ DirectX::XMFLOAT3(0.551356,1.18197,-0.116515), DirectX::XMFLOAT2(0.78125,0.375), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },
		{ DirectX::XMFLOAT3(0.73885,1.207607,0.058399), DirectX::XMFLOAT2(0.84375,0.359375), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },{ DirectX::XMFLOAT3(0.602739,1.191822,-0.150706), DirectX::XMFLOAT2(0.78125,0.359375), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },{ DirectX::XMFLOAT3(0.551356,1.18197,-0.116515), DirectX::XMFLOAT2(0.78125,0.375), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },
		{ DirectX::XMFLOAT3(0.749249,1.146015,0.056279), DirectX::XMFLOAT2(0.84375,0.375), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },{ DirectX::XMFLOAT3(0.697866,1.136163,0.090469), DirectX::XMFLOAT2(0.84375,0.390625), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },{ DirectX::XMFLOAT3(0.613138,1.13023,-0.152825), DirectX::XMFLOAT2(0.78125,0.375), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },
		{ DirectX::XMFLOAT3(0.697866,1.136163,0.090469), DirectX::XMFLOAT2(0.84375,0.390625), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },{ DirectX::XMFLOAT3(0.561755,1.120378,-0.118635), DirectX::XMFLOAT2(0.78125,0.390625), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },{ DirectX::XMFLOAT3(0.613138,1.13023,-0.152825), DirectX::XMFLOAT2(0.78125,0.375), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },
		{ DirectX::XMFLOAT3(0.73885,1.207607,0.058399), DirectX::XMFLOAT2(0.78125,0.34375), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.749249,1.146015,0.056279), DirectX::XMFLOAT2(0.78125,0.359375), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.602739,1.191822,-0.150706), DirectX::XMFLOAT2(0.84375,0.34375), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.749249,1.146015,0.056279), DirectX::XMFLOAT2(0.78125,0.359375), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.613138,1.13023,-0.152825), DirectX::XMFLOAT2(0.84375,0.359375), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.602739,1.191822,-0.150706), DirectX::XMFLOAT2(0.84375,0.34375), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.551356,1.18197,-0.116515), DirectX::XMFLOAT2(0.78125,0.328125), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.561755,1.120378,-0.118635), DirectX::XMFLOAT2(0.78125,0.34375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.687467,1.197755,0.092589), DirectX::XMFLOAT2(0.84375,0.328125), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.561755,1.120378,-0.118635), DirectX::XMFLOAT2(0.78125,0.34375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.697866,1.136163,0.090469), DirectX::XMFLOAT2(0.84375,0.34375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.687467,1.197755,0.092589), DirectX::XMFLOAT2(0.84375,0.328125), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.742328,1.241356,0.016226), DirectX::XMFLOAT2(0.53125,0.34375), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },{ DirectX::XMFLOAT3(0.763126,1.118171,0.011986), DirectX::XMFLOAT2(0.53125,0.375), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },{ DirectX::XMFLOAT3(0.639562,1.221652,0.084606), DirectX::XMFLOAT2(0.5625,0.34375), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },
		{ DirectX::XMFLOAT3(0.763126,1.118171,0.011986), DirectX::XMFLOAT2(0.53125,0.375), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },{ DirectX::XMFLOAT3(0.66036,1.098467,0.080367), DirectX::XMFLOAT2(0.5625,0.375), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },{ DirectX::XMFLOAT3(0.639562,1.221652,0.084606), DirectX::XMFLOAT2(0.5625,0.34375), DirectX::XMFLOAT3(-0.5444,-0.0631,-0.8364) },
		{ DirectX::XMFLOAT3(0.673589,1.225598,0.136882), DirectX::XMFLOAT2(0.3125,0.78125), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },{ DirectX::XMFLOAT3(0.694388,1.102414,0.132643), DirectX::XMFLOAT2(0.3125,0.8125), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },{ DirectX::XMFLOAT3(0.776356,1.245302,0.068502), DirectX::XMFLOAT2(0.34375,0.78125), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },
		{ DirectX::XMFLOAT3(0.694388,1.102414,0.132643), DirectX::XMFLOAT2(0.3125,0.8125), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },{ DirectX::XMFLOAT3(0.797154,1.122118,0.064263), DirectX::XMFLOAT2(0.34375,0.8125), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },{ DirectX::XMFLOAT3(0.776356,1.245302,0.068502), DirectX::XMFLOAT2(0.34375,0.78125), DirectX::XMFLOAT3(0.5444,0.0631,0.8364) },
		{ DirectX::XMFLOAT3(0.673589,1.225598,0.136882), DirectX::XMFLOAT2(0.75,0.28125), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },{ DirectX::XMFLOAT3(0.776356,1.245302,0.068502), DirectX::XMFLOAT2(0.75,0.25), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },{ DirectX::XMFLOAT3(0.639562,1.221652,0.084606), DirectX::XMFLOAT2(0.734375,0.28125), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },
		{ DirectX::XMFLOAT3(0.776356,1.245302,0.068502), DirectX::XMFLOAT2(0.75,0.25), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },{ DirectX::XMFLOAT3(0.742328,1.241356,0.016226), DirectX::XMFLOAT2(0.734375,0.25), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },{ DirectX::XMFLOAT3(0.639562,1.221652,0.084606), DirectX::XMFLOAT2(0.734375,0.28125), DirectX::XMFLOAT3(-0.1664,0.9855,0.0339) },
		{ DirectX::XMFLOAT3(0.797154,1.122118,0.064263), DirectX::XMFLOAT2(0.828125,0.0625), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },{ DirectX::XMFLOAT3(0.694388,1.102414,0.132643), DirectX::XMFLOAT2(0.828125,0.09375), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },{ DirectX::XMFLOAT3(0.763126,1.118171,0.011986), DirectX::XMFLOAT2(0.8125,0.0625), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },
		{ DirectX::XMFLOAT3(0.694388,1.102414,0.132643), DirectX::XMFLOAT2(0.828125,0.09375), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },{ DirectX::XMFLOAT3(0.66036,1.098467,0.080367), DirectX::XMFLOAT2(0.8125,0.09375), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },{ DirectX::XMFLOAT3(0.763126,1.118171,0.011986), DirectX::XMFLOAT2(0.8125,0.0625), DirectX::XMFLOAT3(0.1664,-0.9855,-0.0339) },
		{ DirectX::XMFLOAT3(0.776356,1.245302,0.068502), DirectX::XMFLOAT2(0.671875,0.28125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.797154,1.122118,0.064263), DirectX::XMFLOAT2(0.671875,0.3125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.742328,1.241356,0.016226), DirectX::XMFLOAT2(0.6875,0.28125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.797154,1.122118,0.064263), DirectX::XMFLOAT2(0.671875,0.3125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.763126,1.118171,0.011986), DirectX::XMFLOAT2(0.6875,0.3125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.742328,1.241356,0.016226), DirectX::XMFLOAT2(0.6875,0.28125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.639562,1.221652,0.084606), DirectX::XMFLOAT2(0.609375,0.375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.66036,1.098467,0.080367), DirectX::XMFLOAT2(0.609375,0.40625), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.673589,1.225598,0.136882), DirectX::XMFLOAT2(0.625,0.375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.66036,1.098467,0.080367), DirectX::XMFLOAT2(0.609375,0.40625), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.694388,1.102414,0.132643), DirectX::XMFLOAT2(0.625,0.40625), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.673589,1.225598,0.136882), DirectX::XMFLOAT2(0.625,0.375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.802541,1.31611,0.156821), DirectX::XMFLOAT2(0.515625,0.8125), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },{ DirectX::XMFLOAT3(0.720259,1.234238,0.00957), DirectX::XMFLOAT2(0.515625,0.859375), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },{ DirectX::XMFLOAT3(0.77685,1.311183,0.173916), DirectX::XMFLOAT2(0.523438,0.8125), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },
		{ DirectX::XMFLOAT3(0.720259,1.234238,0.00957), DirectX::XMFLOAT2(0.515625,0.859375), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },{ DirectX::XMFLOAT3(0.694567,1.229311,0.026665), DirectX::XMFLOAT2(0.523438,0.859375), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },{ DirectX::XMFLOAT3(0.77685,1.311183,0.173916), DirectX::XMFLOAT2(0.523438,0.8125), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },
		{ DirectX::XMFLOAT3(0.788183,1.283505,0.182973), DirectX::XMFLOAT2(0.53125,0.8125), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },{ DirectX::XMFLOAT3(0.7059,1.201633,0.035722), DirectX::XMFLOAT2(0.53125,0.859375), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },{ DirectX::XMFLOAT3(0.813874,1.288431,0.165878), DirectX::XMFLOAT2(0.539062,0.8125), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },
		{ DirectX::XMFLOAT3(0.7059,1.201633,0.035722), DirectX::XMFLOAT2(0.53125,0.859375), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },{ DirectX::XMFLOAT3(0.731592,1.206559,0.018627), DirectX::XMFLOAT2(0.539062,0.859375), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },{ DirectX::XMFLOAT3(0.813874,1.288431,0.165878), DirectX::XMFLOAT2(0.539062,0.8125), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },
		{ DirectX::XMFLOAT3(0.788183,1.283505,0.182973), DirectX::XMFLOAT2(0.773438,0.242188), DirectX::XMFLOAT3(0.4388,0.4366,0.7853) },{ DirectX::XMFLOAT3(0.813874,1.288431,0.165878), DirectX::XMFLOAT2(0.773438,0.234375), DirectX::XMFLOAT3(0.4388,0.4366,0.7853) },{ DirectX::XMFLOAT3(0.77685,1.311183,0.173916), DirectX::XMFLOAT2(0.765625,0.242188), DirectX::XMFLOAT3(0.4388,0.4366,0.7853) },
		{ DirectX::XMFLOAT3(0.813874,1.288431,0.165878), DirectX::XMFLOAT2(0.773438,0.234375), DirectX::XMFLOAT3(0.4388,0.4367,0.7853) },{ DirectX::XMFLOAT3(0.802541,1.31611,0.156821), DirectX::XMFLOAT2(0.765625,0.234375), DirectX::XMFLOAT3(0.4388,0.4367,0.7853) },{ DirectX::XMFLOAT3(0.77685,1.311183,0.173916), DirectX::XMFLOAT2(0.765625,0.242188), DirectX::XMFLOAT3(0.4388,0.4367,0.7853) },
		{ DirectX::XMFLOAT3(0.731592,1.206559,0.018627), DirectX::XMFLOAT2(0.804688,0.28125), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },{ DirectX::XMFLOAT3(0.7059,1.201633,0.035722), DirectX::XMFLOAT2(0.804688,0.289062), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },{ DirectX::XMFLOAT3(0.720259,1.234238,0.00957), DirectX::XMFLOAT2(0.796875,0.28125), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },
		{ DirectX::XMFLOAT3(0.7059,1.201633,0.035722), DirectX::XMFLOAT2(0.804688,0.289062), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },{ DirectX::XMFLOAT3(0.694567,1.229311,0.026665), DirectX::XMFLOAT2(0.796875,0.289062), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },{ DirectX::XMFLOAT3(0.720259,1.234238,0.00957), DirectX::XMFLOAT2(0.796875,0.28125), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },
		{ DirectX::XMFLOAT3(0.813874,1.288431,0.165878), DirectX::XMFLOAT2(0.8125,0.515625), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.731592,1.206559,0.018627), DirectX::XMFLOAT2(0.8125,0.5625), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.802541,1.31611,0.156821), DirectX::XMFLOAT2(0.820312,0.515625), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.731592,1.206559,0.018627), DirectX::XMFLOAT2(0.8125,0.5625), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.720259,1.234238,0.00957), DirectX::XMFLOAT2(0.820312,0.5625), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.802541,1.31611,0.156821), DirectX::XMFLOAT2(0.820312,0.515625), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.77685,1.311183,0.173916), DirectX::XMFLOAT2(0.5,0.8125), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.694567,1.229311,0.026665), DirectX::XMFLOAT2(0.5,0.859375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.788183,1.283505,0.182973), DirectX::XMFLOAT2(0.507812,0.8125), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.694567,1.229311,0.026665), DirectX::XMFLOAT2(0.5,0.859375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.7059,1.201633,0.035722), DirectX::XMFLOAT2(0.507812,0.859375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.788183,1.283505,0.182973), DirectX::XMFLOAT2(0.507812,0.8125), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.751158,1.306257,0.191011), DirectX::XMFLOAT2(0.796875,0.8125), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },{ DirectX::XMFLOAT3(0.668876,1.224385,0.04376), DirectX::XMFLOAT2(0.796875,0.859375), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },{ DirectX::XMFLOAT3(0.725467,1.301331,0.208106), DirectX::XMFLOAT2(0.804688,0.8125), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },
		{ DirectX::XMFLOAT3(0.668876,1.224385,0.04376), DirectX::XMFLOAT2(0.796875,0.859375), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },{ DirectX::XMFLOAT3(0.643184,1.219459,0.060855), DirectX::XMFLOAT2(0.804688,0.859375), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },{ DirectX::XMFLOAT3(0.725467,1.301331,0.208106), DirectX::XMFLOAT2(0.804688,0.8125), DirectX::XMFLOAT3(-0.3627,0.8857,-0.2898) },
		{ DirectX::XMFLOAT3(0.7368,1.273653,0.217163), DirectX::XMFLOAT2(0.828125,0.0625), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },{ DirectX::XMFLOAT3(0.654517,1.191781,0.069912), DirectX::XMFLOAT2(0.828125,0.109375), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },{ DirectX::XMFLOAT3(0.762491,1.278579,0.200068), DirectX::XMFLOAT2(0.835938,0.0625), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },
		{ DirectX::XMFLOAT3(0.654517,1.191781,0.069912), DirectX::XMFLOAT2(0.828125,0.109375), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },{ DirectX::XMFLOAT3(0.680209,1.196707,0.052817), DirectX::XMFLOAT2(0.835938,0.109375), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },{ DirectX::XMFLOAT3(0.762491,1.278579,0.200068), DirectX::XMFLOAT2(0.835938,0.0625), DirectX::XMFLOAT3(0.3627,-0.8857,0.2898) },
		{ DirectX::XMFLOAT3(0.7368,1.273653,0.217163), DirectX::XMFLOAT2(0.820312,0.289062), DirectX::XMFLOAT3(0.4388,0.4366,0.7853) },{ DirectX::XMFLOAT3(0.762491,1.278579,0.200068), DirectX::XMFLOAT2(0.820312,0.28125), DirectX::XMFLOAT3(0.4388,0.4366,0.7853) },{ DirectX::XMFLOAT3(0.725467,1.301331,0.208106), DirectX::XMFLOAT2(0.8125,0.289062), DirectX::XMFLOAT3(0.4388,0.4366,0.7853) },
		{ DirectX::XMFLOAT3(0.762491,1.278579,0.200068), DirectX::XMFLOAT2(0.820312,0.28125), DirectX::XMFLOAT3(0.4388,0.4367,0.7853) },{ DirectX::XMFLOAT3(0.751158,1.306257,0.191011), DirectX::XMFLOAT2(0.8125,0.28125), DirectX::XMFLOAT3(0.4388,0.4367,0.7853) },{ DirectX::XMFLOAT3(0.725467,1.301331,0.208106), DirectX::XMFLOAT2(0.8125,0.289062), DirectX::XMFLOAT3(0.4388,0.4367,0.7853) },
		{ DirectX::XMFLOAT3(0.680209,1.196707,0.052817), DirectX::XMFLOAT2(0.820312,0.765625), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },{ DirectX::XMFLOAT3(0.654517,1.191781,0.069912), DirectX::XMFLOAT2(0.820312,0.773438), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },{ DirectX::XMFLOAT3(0.668876,1.224385,0.04376), DirectX::XMFLOAT2(0.8125,0.765625), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },
		{ DirectX::XMFLOAT3(0.654517,1.191781,0.069912), DirectX::XMFLOAT2(0.820312,0.773438), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },{ DirectX::XMFLOAT3(0.643184,1.219459,0.060855), DirectX::XMFLOAT2(0.8125,0.773438), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },{ DirectX::XMFLOAT3(0.668876,1.224385,0.04376), DirectX::XMFLOAT2(0.8125,0.765625), DirectX::XMFLOAT3(-0.4388,-0.4367,-0.7853) },
		{ DirectX::XMFLOAT3(0.762491,1.278579,0.200068), DirectX::XMFLOAT2(0.8125,0.8125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.680209,1.196707,0.052817), DirectX::XMFLOAT2(0.8125,0.859375), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.751158,1.306257,0.191011), DirectX::XMFLOAT2(0.820312,0.8125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.680209,1.196707,0.052817), DirectX::XMFLOAT2(0.8125,0.859375), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.668876,1.224385,0.04376), DirectX::XMFLOAT2(0.820312,0.859375), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.751158,1.306257,0.191011), DirectX::XMFLOAT2(0.820312,0.8125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.725467,1.301331,0.208106), DirectX::XMFLOAT2(0.8125,0.71875), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.643184,1.219459,0.060855), DirectX::XMFLOAT2(0.8125,0.765625), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.7368,1.273653,0.217163), DirectX::XMFLOAT2(0.820312,0.71875), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.643184,1.219459,0.060855), DirectX::XMFLOAT2(0.8125,0.765625), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.654517,1.191781,0.069912), DirectX::XMFLOAT2(0.820312,0.765625), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.7368,1.273653,0.217163), DirectX::XMFLOAT2(0.820312,0.71875), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.741597,1.112755,0.063758), DirectX::XMFLOAT2(0.8125,0.65625), DirectX::XMFLOAT3(0.1660,-0.9855,-0.0345) },{ DirectX::XMFLOAT3(0.809667,1.120564,0.168308), DirectX::XMFLOAT2(0.8125,0.6875), DirectX::XMFLOAT3(0.1660,-0.9855,-0.0345) },{ DirectX::XMFLOAT3(0.715906,1.107829,0.080853), DirectX::XMFLOAT2(0.820312,0.65625), DirectX::XMFLOAT3(0.1660,-0.9855,-0.0345) },
		{ DirectX::XMFLOAT3(0.809667,1.120564,0.168308), DirectX::XMFLOAT2(0.8125,0.6875), DirectX::XMFLOAT3(0.1660,-0.9855,-0.0345) },{ DirectX::XMFLOAT3(0.783975,1.115638,0.185403), DirectX::XMFLOAT2(0.820312,0.6875), DirectX::XMFLOAT3(0.1660,-0.9855,-0.0345) },{ DirectX::XMFLOAT3(0.715906,1.107829,0.080853), DirectX::XMFLOAT2(0.820312,0.65625), DirectX::XMFLOAT3(0.1660,-0.9855,-0.0345) },
		{ DirectX::XMFLOAT3(0.710718,1.138626,0.081931), DirectX::XMFLOAT2(0.828125,0.59375), DirectX::XMFLOAT3(-0.1660,0.9855,0.0345) },{ DirectX::XMFLOAT3(0.778787,1.146436,0.18648), DirectX::XMFLOAT2(0.828125,0.625), DirectX::XMFLOAT3(-0.1660,0.9855,0.0345) },{ DirectX::XMFLOAT3(0.736409,1.143552,0.064836), DirectX::XMFLOAT2(0.835938,0.59375), DirectX::XMFLOAT3(-0.1660,0.9855,0.0345) },
		{ DirectX::XMFLOAT3(0.778787,1.146436,0.18648), DirectX::XMFLOAT2(0.828125,0.625), DirectX::XMFLOAT3(-0.1660,0.9855,0.0345) },{ DirectX::XMFLOAT3(0.804478,1.151362,0.169385), DirectX::XMFLOAT2(0.835938,0.625), DirectX::XMFLOAT3(-0.1660,0.9855,0.0345) },{ DirectX::XMFLOAT3(0.736409,1.143552,0.064836), DirectX::XMFLOAT2(0.835938,0.59375), DirectX::XMFLOAT3(-0.1660,0.9855,0.0345) },
		{ DirectX::XMFLOAT3(0.710718,1.138626,0.081931), DirectX::XMFLOAT2(0.835938,0.210938), DirectX::XMFLOAT3(-0.5446,-0.0625,-0.8364) },{ DirectX::XMFLOAT3(0.736409,1.143552,0.064836), DirectX::XMFLOAT2(0.835938,0.203125), DirectX::XMFLOAT3(-0.5446,-0.0625,-0.8364) },{ DirectX::XMFLOAT3(0.715906,1.107829,0.080853), DirectX::XMFLOAT2(0.828125,0.210938), DirectX::XMFLOAT3(-0.5446,-0.0625,-0.8364) },
		{ DirectX::XMFLOAT3(0.736409,1.143552,0.064836), DirectX::XMFLOAT2(0.835938,0.203125), DirectX::XMFLOAT3(-0.5446,-0.0625,-0.8364) },{ DirectX::XMFLOAT3(0.741597,1.112755,0.063758), DirectX::XMFLOAT2(0.828125,0.203125), DirectX::XMFLOAT3(-0.5446,-0.0625,-0.8364) },{ DirectX::XMFLOAT3(0.715906,1.107829,0.080853), DirectX::XMFLOAT2(0.828125,0.210938), DirectX::XMFLOAT3(-0.5446,-0.0625,-0.8364) },
		{ DirectX::XMFLOAT3(0.804478,1.151362,0.169385), DirectX::XMFLOAT2(0.679688,0.828125), DirectX::XMFLOAT3(0.5446,0.0625,0.8364) },{ DirectX::XMFLOAT3(0.778787,1.146436,0.18648), DirectX::XMFLOAT2(0.679688,0.835938), DirectX::XMFLOAT3(0.5446,0.0625,0.8364) },{ DirectX::XMFLOAT3(0.809667,1.120564,0.168308), DirectX::XMFLOAT2(0.671875,0.828125), DirectX::XMFLOAT3(0.5446,0.0625,0.8364) },
		{ DirectX::XMFLOAT3(0.778787,1.146436,0.18648), DirectX::XMFLOAT2(0.679688,0.835938), DirectX::XMFLOAT3(0.5446,0.0625,0.8364) },{ DirectX::XMFLOAT3(0.783975,1.115638,0.185403), DirectX::XMFLOAT2(0.671875,0.835938), DirectX::XMFLOAT3(0.5446,0.0625,0.8364) },{ DirectX::XMFLOAT3(0.809667,1.120564,0.168308), DirectX::XMFLOAT2(0.671875,0.828125), DirectX::XMFLOAT3(0.5446,0.0625,0.8364) },
		{ DirectX::XMFLOAT3(0.736409,1.143552,0.064836), DirectX::XMFLOAT2(0.828125,0.53125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.804478,1.151362,0.169385), DirectX::XMFLOAT2(0.828125,0.5625), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.741597,1.112755,0.063758), DirectX::XMFLOAT2(0.835938,0.53125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.804478,1.151362,0.169385), DirectX::XMFLOAT2(0.828125,0.5625), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.809667,1.120564,0.168308), DirectX::XMFLOAT2(0.835938,0.5625), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },{ DirectX::XMFLOAT3(0.741597,1.112755,0.063758), DirectX::XMFLOAT2(0.835938,0.53125), DirectX::XMFLOAT3(0.8221,0.1576,-0.547) },
		{ DirectX::XMFLOAT3(0.715906,1.107829,0.080853), DirectX::XMFLOAT2(0.8125,0.59375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.783975,1.115638,0.185403), DirectX::XMFLOAT2(0.8125,0.625), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.710718,1.138626,0.081931), DirectX::XMFLOAT2(0.820312,0.59375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.783975,1.115638,0.185403), DirectX::XMFLOAT2(0.8125,0.625), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.778787,1.146436,0.18648), DirectX::XMFLOAT2(0.820312,0.625), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },{ DirectX::XMFLOAT3(0.710718,1.138626,0.081931), DirectX::XMFLOAT2(0.820312,0.59375), DirectX::XMFLOAT3(-0.8221,-0.1576,0.547) },
		{ DirectX::XMFLOAT3(0.228165,1.128232,-0.160887), DirectX::XMFLOAT2(0.65625,0.578125), DirectX::XMFLOAT3(-0.4196,-0.8479,0.3239) },{ DirectX::XMFLOAT3(0.284809,1.102533,-0.154785), DirectX::XMFLOAT2(0.65625,0.59375), DirectX::XMFLOAT3(-0.4196,-0.8479,0.3239) },{ DirectX::XMFLOAT3(0.231316,1.149141,-0.102072), DirectX::XMFLOAT2(0.671875,0.578125), DirectX::XMFLOAT3(-0.4196,-0.8479,0.3239) },
		{ DirectX::XMFLOAT3(0.284809,1.102533,-0.154785), DirectX::XMFLOAT2(0.65625,0.59375), DirectX::XMFLOAT3(-0.4196,-0.8479,0.3239) },{ DirectX::XMFLOAT3(0.287961,1.123441,-0.095971), DirectX::XMFLOAT2(0.671875,0.59375), DirectX::XMFLOAT3(-0.4196,-0.8479,0.3239) },{ DirectX::XMFLOAT3(0.231316,1.149141,-0.102072), DirectX::XMFLOAT2(0.671875,0.578125), DirectX::XMFLOAT3(-0.4196,-0.8479,0.3239) },
		{ DirectX::XMFLOAT3(0.336216,1.361127,-0.183054), DirectX::XMFLOAT2(0.6875,0.5), DirectX::XMFLOAT3(0.4196,0.8479,-0.3239) },{ DirectX::XMFLOAT3(0.39286,1.335427,-0.176953), DirectX::XMFLOAT2(0.6875,0.515625), DirectX::XMFLOAT3(0.4196,0.8479,-0.3239) },{ DirectX::XMFLOAT3(0.333065,1.340218,-0.241868), DirectX::XMFLOAT2(0.703125,0.5), DirectX::XMFLOAT3(0.4196,0.8479,-0.3239) },
		{ DirectX::XMFLOAT3(0.39286,1.335427,-0.176953), DirectX::XMFLOAT2(0.6875,0.515625), DirectX::XMFLOAT3(0.4196,0.8479,-0.3239) },{ DirectX::XMFLOAT3(0.389709,1.314519,-0.235767), DirectX::XMFLOAT2(0.703125,0.515625), DirectX::XMFLOAT3(0.4196,0.8479,-0.3239) },{ DirectX::XMFLOAT3(0.333065,1.340218,-0.241868), DirectX::XMFLOAT2(0.703125,0.5), DirectX::XMFLOAT3(0.4196,0.8479,-0.3239) },
		{ DirectX::XMFLOAT3(0.336216,1.361127,-0.183054), DirectX::XMFLOAT2(0.84375,0.4375), DirectX::XMFLOAT3(-0.9063,0.4112,-0.0976) },{ DirectX::XMFLOAT3(0.333065,1.340218,-0.241868), DirectX::XMFLOAT2(0.84375,0.421875), DirectX::XMFLOAT3(-0.9063,0.4112,-0.0976) },{ DirectX::XMFLOAT3(0.231316,1.149141,-0.102072), DirectX::XMFLOAT2(0.78125,0.4375), DirectX::XMFLOAT3(-0.9063,0.4112,-0.0976) },
		{ DirectX::XMFLOAT3(0.333065,1.340218,-0.241868), DirectX::XMFLOAT2(0.84375,0.421875), DirectX::XMFLOAT3(-0.9063,0.4112,-0.0976) },{ DirectX::XMFLOAT3(0.228165,1.128232,-0.160887), DirectX::XMFLOAT2(0.78125,0.421875), DirectX::XMFLOAT3(-0.9063,0.4112,-0.0976) },{ DirectX::XMFLOAT3(0.231316,1.149141,-0.102072), DirectX::XMFLOAT2(0.78125,0.4375), DirectX::XMFLOAT3(-0.9063,0.4112,-0.0976) },
		{ DirectX::XMFLOAT3(0.389709,1.314519,-0.235767), DirectX::XMFLOAT2(0.84375,0.4375), DirectX::XMFLOAT3(0.9063,-0.4112,0.0976) },{ DirectX::XMFLOAT3(0.39286,1.335427,-0.176953), DirectX::XMFLOAT2(0.84375,0.453125), DirectX::XMFLOAT3(0.9063,-0.4112,0.0976) },{ DirectX::XMFLOAT3(0.284809,1.102533,-0.154785), DirectX::XMFLOAT2(0.78125,0.4375), DirectX::XMFLOAT3(0.9063,-0.4112,0.0976) },
		{ DirectX::XMFLOAT3(0.39286,1.335427,-0.176953), DirectX::XMFLOAT2(0.84375,0.453125), DirectX::XMFLOAT3(0.9063,-0.4112,0.0976) },{ DirectX::XMFLOAT3(0.287961,1.123441,-0.095971), DirectX::XMFLOAT2(0.78125,0.453125), DirectX::XMFLOAT3(0.9063,-0.4112,0.0976) },{ DirectX::XMFLOAT3(0.284809,1.102533,-0.154785), DirectX::XMFLOAT2(0.78125,0.4375), DirectX::XMFLOAT3(0.9063,-0.4112,0.0976) },
		{ DirectX::XMFLOAT3(0.333065,1.340218,-0.241868), DirectX::XMFLOAT2(0.78125,0.40625), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.389709,1.314519,-0.235767), DirectX::XMFLOAT2(0.78125,0.421875), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.228165,1.128232,-0.160887), DirectX::XMFLOAT2(0.84375,0.40625), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.389709,1.314519,-0.235767), DirectX::XMFLOAT2(0.78125,0.421875), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.284809,1.102533,-0.154785), DirectX::XMFLOAT2(0.84375,0.421875), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },{ DirectX::XMFLOAT3(0.228165,1.128232,-0.160887), DirectX::XMFLOAT2(0.84375,0.40625), DirectX::XMFLOAT3(-0.0504,-0.3345,-0.941) },
		{ DirectX::XMFLOAT3(0.231316,1.149141,-0.102072), DirectX::XMFLOAT2(0.78125,0.390625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.287961,1.123441,-0.095971), DirectX::XMFLOAT2(0.78125,0.40625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.336216,1.361127,-0.183054), DirectX::XMFLOAT2(0.84375,0.390625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.287961,1.123441,-0.095971), DirectX::XMFLOAT2(0.78125,0.40625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.39286,1.335427,-0.176953), DirectX::XMFLOAT2(0.84375,0.40625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },{ DirectX::XMFLOAT3(0.336216,1.361127,-0.183054), DirectX::XMFLOAT2(0.84375,0.390625), DirectX::XMFLOAT3(0.0504,0.3345,0.941) },
		{ DirectX::XMFLOAT3(0.117727,1.226723,0.085124), DirectX::XMFLOAT2(0.703125,0.5), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },{ DirectX::XMFLOAT3(0.122429,1.165166,0.094866), DirectX::XMFLOAT2(0.703125,0.515625), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },{ DirectX::XMFLOAT3(0.173711,1.235188,0.111589), DirectX::XMFLOAT2(0.71875,0.5), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },
		{ DirectX::XMFLOAT3(0.122429,1.165166,0.094866), DirectX::XMFLOAT2(0.703125,0.515625), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },{ DirectX::XMFLOAT3(0.178413,1.173631,0.121331), DirectX::XMFLOAT2(0.71875,0.515625), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },{ DirectX::XMFLOAT3(0.173711,1.235188,0.111589), DirectX::XMFLOAT2(0.71875,0.5), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },
		{ DirectX::XMFLOAT3(0.283251,1.208247,-0.111514), DirectX::XMFLOAT2(0.75,0.6875), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },{ DirectX::XMFLOAT3(0.287953,1.14669,-0.101773), DirectX::XMFLOAT2(0.75,0.703125), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },{ DirectX::XMFLOAT3(0.227267,1.199782,-0.137979), DirectX::XMFLOAT2(0.765625,0.6875), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },
		{ DirectX::XMFLOAT3(0.287953,1.14669,-0.101773), DirectX::XMFLOAT2(0.75,0.703125), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },{ DirectX::XMFLOAT3(0.231969,1.138226,-0.128238), DirectX::XMFLOAT2(0.765625,0.703125), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },{ DirectX::XMFLOAT3(0.227267,1.199782,-0.137979), DirectX::XMFLOAT2(0.765625,0.6875), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },
		{ DirectX::XMFLOAT3(0.283251,1.208247,-0.111514), DirectX::XMFLOAT2(0.84375,0.578125), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },{ DirectX::XMFLOAT3(0.227267,1.199782,-0.137979), DirectX::XMFLOAT2(0.84375,0.5625), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },{ DirectX::XMFLOAT3(0.173711,1.235188,0.111589), DirectX::XMFLOAT2(0.78125,0.578125), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },
		{ DirectX::XMFLOAT3(0.227267,1.199782,-0.137979), DirectX::XMFLOAT2(0.84375,0.5625), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },{ DirectX::XMFLOAT3(0.117727,1.226723,0.085124), DirectX::XMFLOAT2(0.78125,0.5625), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },{ DirectX::XMFLOAT3(0.173711,1.235188,0.111589), DirectX::XMFLOAT2(0.78125,0.578125), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },
		{ DirectX::XMFLOAT3(0.231969,1.138226,-0.128238), DirectX::XMFLOAT2(0.84375,0.578125), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },{ DirectX::XMFLOAT3(0.287953,1.14669,-0.101773), DirectX::XMFLOAT2(0.84375,0.59375), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },{ DirectX::XMFLOAT3(0.122429,1.165166,0.094866), DirectX::XMFLOAT2(0.78125,0.578125), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },
		{ DirectX::XMFLOAT3(0.287953,1.14669,-0.101773), DirectX::XMFLOAT2(0.84375,0.59375), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },{ DirectX::XMFLOAT3(0.178413,1.173631,0.121331), DirectX::XMFLOAT2(0.78125,0.59375), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },{ DirectX::XMFLOAT3(0.122429,1.165166,0.094866), DirectX::XMFLOAT2(0.78125,0.578125), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },
		{ DirectX::XMFLOAT3(0.227267,1.199782,-0.137979), DirectX::XMFLOAT2(0.78125,0.46875), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.231969,1.138226,-0.128238), DirectX::XMFLOAT2(0.78125,0.484375), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.117727,1.226723,0.085124), DirectX::XMFLOAT2(0.84375,0.46875), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.231969,1.138226,-0.128238), DirectX::XMFLOAT2(0.78125,0.484375), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.122429,1.165166,0.094866), DirectX::XMFLOAT2(0.84375,0.484375), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.117727,1.226723,0.085124), DirectX::XMFLOAT2(0.84375,0.46875), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.173711,1.235188,0.111589), DirectX::XMFLOAT2(0.78125,0.453125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.178413,1.173631,0.121331), DirectX::XMFLOAT2(0.78125,0.46875), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.283251,1.208247,-0.111514), DirectX::XMFLOAT2(0.84375,0.453125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.178413,1.173631,0.121331), DirectX::XMFLOAT2(0.78125,0.46875), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.287953,1.14669,-0.101773), DirectX::XMFLOAT2(0.84375,0.46875), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.283251,1.208247,-0.111514), DirectX::XMFLOAT2(0.84375,0.453125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.073691,1.256636,0.094909), DirectX::XMFLOAT2(0.421875,0.78125), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },{ DirectX::XMFLOAT3(0.083095,1.133523,0.114392), DirectX::XMFLOAT2(0.421875,0.8125), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },{ DirectX::XMFLOAT3(0.185659,1.273566,0.147839), DirectX::XMFLOAT2(0.453125,0.78125), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },
		{ DirectX::XMFLOAT3(0.083095,1.133523,0.114392), DirectX::XMFLOAT2(0.421875,0.8125), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },{ DirectX::XMFLOAT3(0.195063,1.150452,0.167322), DirectX::XMFLOAT2(0.453125,0.8125), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },{ DirectX::XMFLOAT3(0.185659,1.273566,0.147839), DirectX::XMFLOAT2(0.453125,0.78125), DirectX::XMFLOAT3(-0.4382,0.1078,0.8924) },
		{ DirectX::XMFLOAT3(0.213044,1.26683,0.092063), DirectX::XMFLOAT2(0.78125,0.59375), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },{ DirectX::XMFLOAT3(0.222448,1.143717,0.111546), DirectX::XMFLOAT2(0.78125,0.625), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },{ DirectX::XMFLOAT3(0.101076,1.249901,0.039133), DirectX::XMFLOAT2(0.8125,0.59375), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },
		{ DirectX::XMFLOAT3(0.222448,1.143717,0.111546), DirectX::XMFLOAT2(0.78125,0.625), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },{ DirectX::XMFLOAT3(0.11048,1.126788,0.058616), DirectX::XMFLOAT2(0.8125,0.625), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },{ DirectX::XMFLOAT3(0.101076,1.249901,0.039133), DirectX::XMFLOAT2(0.8125,0.59375), DirectX::XMFLOAT3(0.4382,-0.1078,-0.8924) },
		{ DirectX::XMFLOAT3(0.213044,1.26683,0.092063), DirectX::XMFLOAT2(0.234375,0.84375), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },{ DirectX::XMFLOAT3(0.101076,1.249901,0.039133), DirectX::XMFLOAT2(0.234375,0.8125), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },{ DirectX::XMFLOAT3(0.185659,1.273566,0.147839), DirectX::XMFLOAT2(0.21875,0.84375), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },
		{ DirectX::XMFLOAT3(0.101076,1.249901,0.039133), DirectX::XMFLOAT2(0.234375,0.8125), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },{ DirectX::XMFLOAT3(0.073691,1.256636,0.094909), DirectX::XMFLOAT2(0.21875,0.8125), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },{ DirectX::XMFLOAT3(0.185659,1.273566,0.147839), DirectX::XMFLOAT2(0.21875,0.84375), DirectX::XMFLOAT3(-0.0752,0.9849,-0.1559) },
		{ DirectX::XMFLOAT3(0.11048,1.126788,0.058616), DirectX::XMFLOAT2(0.25,0.8125), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },{ DirectX::XMFLOAT3(0.222448,1.143717,0.111546), DirectX::XMFLOAT2(0.25,0.84375), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },{ DirectX::XMFLOAT3(0.083095,1.133523,0.114392), DirectX::XMFLOAT2(0.234375,0.8125), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },
		{ DirectX::XMFLOAT3(0.222448,1.143717,0.111546), DirectX::XMFLOAT2(0.25,0.84375), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },{ DirectX::XMFLOAT3(0.195063,1.150452,0.167322), DirectX::XMFLOAT2(0.234375,0.84375), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },{ DirectX::XMFLOAT3(0.083095,1.133523,0.114392), DirectX::XMFLOAT2(0.234375,0.8125), DirectX::XMFLOAT3(0.0752,-0.9849,0.1559) },
		{ DirectX::XMFLOAT3(0.101076,1.249901,0.039133), DirectX::XMFLOAT2(0.203125,0.8125), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.11048,1.126788,0.058616), DirectX::XMFLOAT2(0.203125,0.84375), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.073691,1.256636,0.094909), DirectX::XMFLOAT2(0.21875,0.8125), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.11048,1.126788,0.058616), DirectX::XMFLOAT2(0.203125,0.84375), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.083095,1.133523,0.114392), DirectX::XMFLOAT2(0.21875,0.84375), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.073691,1.256636,0.094909), DirectX::XMFLOAT2(0.21875,0.8125), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.185659,1.273566,0.147839), DirectX::XMFLOAT2(0.1875,0.8125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.195063,1.150452,0.167322), DirectX::XMFLOAT2(0.1875,0.84375), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.213044,1.26683,0.092063), DirectX::XMFLOAT2(0.203125,0.8125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.195063,1.150452,0.167322), DirectX::XMFLOAT2(0.1875,0.84375), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.222448,1.143717,0.111546), DirectX::XMFLOAT2(0.203125,0.84375), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.213044,1.26683,0.092063), DirectX::XMFLOAT2(0.203125,0.8125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.038885,1.309076,0.188664), DirectX::XMFLOAT2(0.828125,0.15625), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },{ DirectX::XMFLOAT3(0.120148,1.21941,0.045442), DirectX::XMFLOAT2(0.828125,0.203125), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },{ DirectX::XMFLOAT3(0.066877,1.313308,0.201897), DirectX::XMFLOAT2(0.835938,0.15625), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },
		{ DirectX::XMFLOAT3(0.120148,1.21941,0.045442), DirectX::XMFLOAT2(0.828125,0.203125), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },{ DirectX::XMFLOAT3(0.14814,1.223643,0.058674), DirectX::XMFLOAT2(0.835938,0.203125), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },{ DirectX::XMFLOAT3(0.066877,1.313308,0.201897), DirectX::XMFLOAT2(0.835938,0.15625), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },
		{ DirectX::XMFLOAT3(0.069973,1.340425,0.186676), DirectX::XMFLOAT2(0.375,0.828125), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },{ DirectX::XMFLOAT3(0.151235,1.250759,0.043454), DirectX::XMFLOAT2(0.375,0.875), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },{ DirectX::XMFLOAT3(0.041981,1.336193,0.173444), DirectX::XMFLOAT2(0.382812,0.828125), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },
		{ DirectX::XMFLOAT3(0.151235,1.250759,0.043454), DirectX::XMFLOAT2(0.375,0.875), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },{ DirectX::XMFLOAT3(0.123243,1.246527,0.030221), DirectX::XMFLOAT2(0.382812,0.875), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },{ DirectX::XMFLOAT3(0.041981,1.336193,0.173444), DirectX::XMFLOAT2(0.382812,0.828125), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },
		{ DirectX::XMFLOAT3(0.069973,1.340425,0.186676), DirectX::XMFLOAT2(0.695312,0.835938), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },{ DirectX::XMFLOAT3(0.041981,1.336193,0.173444), DirectX::XMFLOAT2(0.695312,0.828125), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },{ DirectX::XMFLOAT3(0.066877,1.313308,0.201897), DirectX::XMFLOAT2(0.6875,0.835938), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },
		{ DirectX::XMFLOAT3(0.041981,1.336193,0.173444), DirectX::XMFLOAT2(0.695312,0.828125), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },{ DirectX::XMFLOAT3(0.038885,1.309076,0.188664), DirectX::XMFLOAT2(0.6875,0.828125), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },{ DirectX::XMFLOAT3(0.066877,1.313308,0.201897), DirectX::XMFLOAT2(0.6875,0.835938), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },
		{ DirectX::XMFLOAT3(0.123243,1.246527,0.030221), DirectX::XMFLOAT2(0.835938,0.6875), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },{ DirectX::XMFLOAT3(0.151235,1.250759,0.043454), DirectX::XMFLOAT2(0.835938,0.695312), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },{ DirectX::XMFLOAT3(0.120148,1.21941,0.045442), DirectX::XMFLOAT2(0.828125,0.6875), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },
		{ DirectX::XMFLOAT3(0.151235,1.250759,0.043454), DirectX::XMFLOAT2(0.835938,0.695312), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },{ DirectX::XMFLOAT3(0.14814,1.223643,0.058674), DirectX::XMFLOAT2(0.828125,0.695312), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },{ DirectX::XMFLOAT3(0.120148,1.21941,0.045442), DirectX::XMFLOAT2(0.828125,0.6875), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },
		{ DirectX::XMFLOAT3(0.041981,1.336193,0.173444), DirectX::XMFLOAT2(0.828125,0.25), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.123243,1.246527,0.030221), DirectX::XMFLOAT2(0.828125,0.296875), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.038885,1.309076,0.188664), DirectX::XMFLOAT2(0.835938,0.25), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.123243,1.246527,0.030221), DirectX::XMFLOAT2(0.828125,0.296875), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.120148,1.21941,0.045442), DirectX::XMFLOAT2(0.835938,0.296875), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.038885,1.309076,0.188664), DirectX::XMFLOAT2(0.835938,0.25), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.066877,1.313308,0.201897), DirectX::XMFLOAT2(0.828125,0.109375), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.14814,1.223643,0.058674), DirectX::XMFLOAT2(0.828125,0.15625), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.069973,1.340425,0.186676), DirectX::XMFLOAT2(0.835938,0.109375), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.14814,1.223643,0.058674), DirectX::XMFLOAT2(0.828125,0.15625), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.151235,1.250759,0.043454), DirectX::XMFLOAT2(0.835938,0.15625), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.069973,1.340425,0.186676), DirectX::XMFLOAT2(0.835938,0.109375), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.094869,1.317541,0.215129), DirectX::XMFLOAT2(0.828125,0.484375), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },{ DirectX::XMFLOAT3(0.176132,1.227875,0.071907), DirectX::XMFLOAT2(0.828125,0.53125), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },{ DirectX::XMFLOAT3(0.122861,1.321773,0.228362), DirectX::XMFLOAT2(0.835938,0.484375), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },
		{ DirectX::XMFLOAT3(0.176132,1.227875,0.071907), DirectX::XMFLOAT2(0.828125,0.53125), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },{ DirectX::XMFLOAT3(0.204124,1.232107,0.085139), DirectX::XMFLOAT2(0.835938,0.53125), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },{ DirectX::XMFLOAT3(0.122861,1.321773,0.228362), DirectX::XMFLOAT2(0.835938,0.484375), DirectX::XMFLOAT3(-0.0990,-0.8677,0.4871) },
		{ DirectX::XMFLOAT3(0.125957,1.34889,0.213141), DirectX::XMFLOAT2(0.609375,0.828125), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },{ DirectX::XMFLOAT3(0.207219,1.259224,0.069919), DirectX::XMFLOAT2(0.609375,0.875), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },{ DirectX::XMFLOAT3(0.097965,1.344657,0.199909), DirectX::XMFLOAT2(0.617188,0.828125), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },
		{ DirectX::XMFLOAT3(0.207219,1.259224,0.069919), DirectX::XMFLOAT2(0.609375,0.875), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },{ DirectX::XMFLOAT3(0.179227,1.254992,0.056686), DirectX::XMFLOAT2(0.617188,0.875), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },{ DirectX::XMFLOAT3(0.097965,1.344657,0.199909), DirectX::XMFLOAT2(0.617188,0.828125), DirectX::XMFLOAT3(0.0990,0.8677,-0.4871) },
		{ DirectX::XMFLOAT3(0.125957,1.34889,0.213141), DirectX::XMFLOAT2(0.710938,0.835938), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },{ DirectX::XMFLOAT3(0.097965,1.344657,0.199909), DirectX::XMFLOAT2(0.710938,0.828125), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },{ DirectX::XMFLOAT3(0.122861,1.321773,0.228362), DirectX::XMFLOAT2(0.703125,0.835938), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },
		{ DirectX::XMFLOAT3(0.097965,1.344657,0.199909), DirectX::XMFLOAT2(0.710938,0.828125), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },{ DirectX::XMFLOAT3(0.094869,1.317541,0.215129), DirectX::XMFLOAT2(0.703125,0.828125), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },{ DirectX::XMFLOAT3(0.122861,1.321773,0.228362), DirectX::XMFLOAT2(0.703125,0.835938), DirectX::XMFLOAT3(-0.4334,0.4782,0.7639) },
		{ DirectX::XMFLOAT3(0.179227,1.254992,0.056686), DirectX::XMFLOAT2(0.835938,0.703125), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },{ DirectX::XMFLOAT3(0.207219,1.259224,0.069919), DirectX::XMFLOAT2(0.835938,0.710938), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },{ DirectX::XMFLOAT3(0.176132,1.227875,0.071907), DirectX::XMFLOAT2(0.828125,0.703125), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },
		{ DirectX::XMFLOAT3(0.207219,1.259224,0.069919), DirectX::XMFLOAT2(0.835938,0.710938), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },{ DirectX::XMFLOAT3(0.204124,1.232107,0.085139), DirectX::XMFLOAT2(0.828125,0.710938), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },{ DirectX::XMFLOAT3(0.176132,1.227875,0.071907), DirectX::XMFLOAT2(0.828125,0.703125), DirectX::XMFLOAT3(0.4334,-0.4782,-0.7639) },
		{ DirectX::XMFLOAT3(0.097965,1.344657,0.199909), DirectX::XMFLOAT2(0.59375,0.828125), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.179227,1.254992,0.056686), DirectX::XMFLOAT2(0.59375,0.875), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.094869,1.317541,0.215129), DirectX::XMFLOAT2(0.601562,0.828125), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.179227,1.254992,0.056686), DirectX::XMFLOAT2(0.59375,0.875), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.176132,1.227875,0.071907), DirectX::XMFLOAT2(0.601562,0.875), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.094869,1.317541,0.215129), DirectX::XMFLOAT2(0.601562,0.828125), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.122861,1.321773,0.228362), DirectX::XMFLOAT2(0.390625,0.828125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.204124,1.232107,0.085139), DirectX::XMFLOAT2(0.390625,0.875), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.125957,1.34889,0.213141), DirectX::XMFLOAT2(0.398438,0.828125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.204124,1.232107,0.085139), DirectX::XMFLOAT2(0.390625,0.875), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.207219,1.259224,0.069919), DirectX::XMFLOAT2(0.398438,0.875), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.125957,1.34889,0.213141), DirectX::XMFLOAT2(0.398438,0.828125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.136415,1.167284,0.101501), DirectX::XMFLOAT2(0.640625,0.828125), DirectX::XMFLOAT3(-0.0758,0.9851,-0.1547) },{ DirectX::XMFLOAT3(0.081658,1.180589,0.213079), DirectX::XMFLOAT2(0.640625,0.859375), DirectX::XMFLOAT3(-0.0758,0.9851,-0.1547) },{ DirectX::XMFLOAT3(0.164407,1.171517,0.114733), DirectX::XMFLOAT2(0.648438,0.828125), DirectX::XMFLOAT3(-0.0758,0.9851,-0.1547) },
		{ DirectX::XMFLOAT3(0.081658,1.180589,0.213079), DirectX::XMFLOAT2(0.640625,0.859375), DirectX::XMFLOAT3(-0.0758,0.9851,-0.1547) },{ DirectX::XMFLOAT3(0.10965,1.184822,0.226311), DirectX::XMFLOAT2(0.648438,0.859375), DirectX::XMFLOAT3(-0.0758,0.9851,-0.1547) },{ DirectX::XMFLOAT3(0.164407,1.171517,0.114733), DirectX::XMFLOAT2(0.648438,0.828125), DirectX::XMFLOAT3(-0.0758,0.9851,-0.1547) },
		{ DirectX::XMFLOAT3(0.166777,1.140734,0.119567), DirectX::XMFLOAT2(0.828125,0.65625), DirectX::XMFLOAT3(0.0758,-0.9851,0.1547) },{ DirectX::XMFLOAT3(0.11202,1.154039,0.231145), DirectX::XMFLOAT2(0.828125,0.6875), DirectX::XMFLOAT3(0.0758,-0.9851,0.1547) },{ DirectX::XMFLOAT3(0.138785,1.136502,0.106334), DirectX::XMFLOAT2(0.835938,0.65625), DirectX::XMFLOAT3(0.0758,-0.9851,0.1547) },
		{ DirectX::XMFLOAT3(0.11202,1.154039,0.231145), DirectX::XMFLOAT2(0.828125,0.6875), DirectX::XMFLOAT3(0.0758,-0.9851,0.1547) },{ DirectX::XMFLOAT3(0.084028,1.149806,0.217912), DirectX::XMFLOAT2(0.835938,0.6875), DirectX::XMFLOAT3(0.0758,-0.9851,0.1547) },{ DirectX::XMFLOAT3(0.138785,1.136502,0.106334), DirectX::XMFLOAT2(0.835938,0.65625), DirectX::XMFLOAT3(0.0758,-0.9851,0.1547) },
		{ DirectX::XMFLOAT3(0.166777,1.140734,0.119567), DirectX::XMFLOAT2(0.835938,0.726562), DirectX::XMFLOAT3(0.4381,-0.1064,-0.8926) },{ DirectX::XMFLOAT3(0.138785,1.136502,0.106334), DirectX::XMFLOAT2(0.835938,0.71875), DirectX::XMFLOAT3(0.4381,-0.1064,-0.8926) },{ DirectX::XMFLOAT3(0.164407,1.171517,0.114733), DirectX::XMFLOAT2(0.828125,0.726562), DirectX::XMFLOAT3(0.4381,-0.1064,-0.8926) },
		{ DirectX::XMFLOAT3(0.138785,1.136502,0.106334), DirectX::XMFLOAT2(0.835938,0.71875), DirectX::XMFLOAT3(0.4381,-0.1064,-0.8926) },{ DirectX::XMFLOAT3(0.136415,1.167284,0.101501), DirectX::XMFLOAT2(0.828125,0.71875), DirectX::XMFLOAT3(0.4381,-0.1064,-0.8926) },{ DirectX::XMFLOAT3(0.164407,1.171517,0.114733), DirectX::XMFLOAT2(0.828125,0.726562), DirectX::XMFLOAT3(0.4381,-0.1064,-0.8926) },
		{ DirectX::XMFLOAT3(0.084028,1.149806,0.217912), DirectX::XMFLOAT2(0.835938,0.734375), DirectX::XMFLOAT3(-0.4381,0.1064,0.8926) },{ DirectX::XMFLOAT3(0.11202,1.154039,0.231145), DirectX::XMFLOAT2(0.835938,0.742188), DirectX::XMFLOAT3(-0.4381,0.1064,0.8926) },{ DirectX::XMFLOAT3(0.081658,1.180589,0.213079), DirectX::XMFLOAT2(0.828125,0.734375), DirectX::XMFLOAT3(-0.4381,0.1064,0.8926) },
		{ DirectX::XMFLOAT3(0.11202,1.154039,0.231145), DirectX::XMFLOAT2(0.835938,0.742188), DirectX::XMFLOAT3(-0.4381,0.1064,0.8926) },{ DirectX::XMFLOAT3(0.10965,1.184822,0.226311), DirectX::XMFLOAT2(0.828125,0.742188), DirectX::XMFLOAT3(-0.4381,0.1064,0.8926) },{ DirectX::XMFLOAT3(0.081658,1.180589,0.213079), DirectX::XMFLOAT2(0.828125,0.734375), DirectX::XMFLOAT3(-0.4381,0.1064,0.8926) },
		{ DirectX::XMFLOAT3(0.138785,1.136502,0.106334), DirectX::XMFLOAT2(0.65625,0.828125), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.084028,1.149806,0.217912), DirectX::XMFLOAT2(0.65625,0.859375), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.136415,1.167284,0.101501), DirectX::XMFLOAT2(0.664062,0.828125), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.084028,1.149806,0.217912), DirectX::XMFLOAT2(0.65625,0.859375), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.081658,1.180589,0.213079), DirectX::XMFLOAT2(0.664062,0.859375), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },{ DirectX::XMFLOAT3(0.136415,1.167284,0.101501), DirectX::XMFLOAT2(0.664062,0.828125), DirectX::XMFLOAT3(-0.8957,-0.1354,-0.4234) },
		{ DirectX::XMFLOAT3(0.164407,1.171517,0.114733), DirectX::XMFLOAT2(0.625,0.828125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.10965,1.184822,0.226311), DirectX::XMFLOAT2(0.625,0.859375), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.166777,1.140734,0.119567), DirectX::XMFLOAT2(0.632812,0.828125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.10965,1.184822,0.226311), DirectX::XMFLOAT2(0.625,0.859375), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.11202,1.154039,0.231145), DirectX::XMFLOAT2(0.632812,0.859375), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },{ DirectX::XMFLOAT3(0.166777,1.140734,0.119567), DirectX::XMFLOAT2(0.632812,0.828125), DirectX::XMFLOAT3(0.8957,0.1354,0.4234) },
		{ DirectX::XMFLOAT3(0.35339,1.686904,-0.051863), DirectX::XMFLOAT2(0.734375,0.09375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.334184,1.477492,-0.28302), DirectX::XMFLOAT2(0.734375,0.171875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.351329,1.594364,0.032144), DirectX::XMFLOAT2(0.765625,0.09375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.334184,1.477492,-0.28302), DirectX::XMFLOAT2(0.734375,0.171875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.332123,1.384951,-0.199014), DirectX::XMFLOAT2(0.765625,0.171875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.351329,1.594364,0.032144), DirectX::XMFLOAT2(0.765625,0.09375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.538449,1.584334,0.025684), DirectX::XMFLOAT2(0.734375,0.171875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.519243,1.374921,-0.205473), DirectX::XMFLOAT2(0.734375,0.25), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.54051,1.676874,-0.058323), DirectX::XMFLOAT2(0.765625,0.171875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.519243,1.374921,-0.205473), DirectX::XMFLOAT2(0.734375,0.25), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.521304,1.467461,-0.28948), DirectX::XMFLOAT2(0.765625,0.25), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.54051,1.676874,-0.058323), DirectX::XMFLOAT2(0.765625,0.171875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.538449,1.584334,0.025684), DirectX::XMFLOAT2(0.8125,0.75), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },{ DirectX::XMFLOAT3(0.54051,1.676874,-0.058323), DirectX::XMFLOAT2(0.8125,0.71875), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },{ DirectX::XMFLOAT3(0.351329,1.594364,0.032144), DirectX::XMFLOAT2(0.765625,0.75), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },
		{ DirectX::XMFLOAT3(0.54051,1.676874,-0.058323), DirectX::XMFLOAT2(0.8125,0.71875), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },{ DirectX::XMFLOAT3(0.35339,1.686904,-0.051863), DirectX::XMFLOAT2(0.765625,0.71875), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },{ DirectX::XMFLOAT3(0.351329,1.594364,0.032144), DirectX::XMFLOAT2(0.765625,0.75), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },
		{ DirectX::XMFLOAT3(0.521304,1.467461,-0.28948), DirectX::XMFLOAT2(0.8125,0.75), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },{ DirectX::XMFLOAT3(0.519243,1.374921,-0.205473), DirectX::XMFLOAT2(0.8125,0.78125), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },{ DirectX::XMFLOAT3(0.334184,1.477492,-0.28302), DirectX::XMFLOAT2(0.765625,0.75), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },
		{ DirectX::XMFLOAT3(0.519243,1.374921,-0.205473), DirectX::XMFLOAT2(0.8125,0.78125), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },{ DirectX::XMFLOAT3(0.332123,1.384951,-0.199014), DirectX::XMFLOAT2(0.765625,0.78125), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },{ DirectX::XMFLOAT3(0.334184,1.477492,-0.28302), DirectX::XMFLOAT2(0.765625,0.75), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },
		{ DirectX::XMFLOAT3(0.54051,1.676874,-0.058323), DirectX::XMFLOAT2(0.375,0.6875), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },{ DirectX::XMFLOAT3(0.521304,1.467461,-0.28948), DirectX::XMFLOAT2(0.375,0.765625), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },{ DirectX::XMFLOAT3(0.35339,1.686904,-0.051863), DirectX::XMFLOAT2(0.421875,0.6875), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },
		{ DirectX::XMFLOAT3(0.521304,1.467461,-0.28948), DirectX::XMFLOAT2(0.375,0.765625), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },{ DirectX::XMFLOAT3(0.334184,1.477492,-0.28302), DirectX::XMFLOAT2(0.421875,0.765625), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },{ DirectX::XMFLOAT3(0.35339,1.686904,-0.051863), DirectX::XMFLOAT2(0.421875,0.6875), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },
		{ DirectX::XMFLOAT3(0.351329,1.594364,0.032144), DirectX::XMFLOAT2(0.671875,0.578125), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },{ DirectX::XMFLOAT3(0.332123,1.384951,-0.199014), DirectX::XMFLOAT2(0.671875,0.65625), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },{ DirectX::XMFLOAT3(0.538449,1.584334,0.025684), DirectX::XMFLOAT2(0.71875,0.578125), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },
		{ DirectX::XMFLOAT3(0.332123,1.384951,-0.199014), DirectX::XMFLOAT2(0.671875,0.65625), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },{ DirectX::XMFLOAT3(0.519243,1.374921,-0.205473), DirectX::XMFLOAT2(0.71875,0.65625), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },{ DirectX::XMFLOAT3(0.538449,1.584334,0.025684), DirectX::XMFLOAT2(0.71875,0.578125), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },
		{ DirectX::XMFLOAT3(0.300714,1.847358,-0.012767), DirectX::XMFLOAT2(0.75,0.484375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.293808,1.722717,-0.019249), DirectX::XMFLOAT2(0.75,0.515625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.308608,1.833943,0.236748), DirectX::XMFLOAT2(0.8125,0.484375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.293808,1.722717,-0.019249), DirectX::XMFLOAT2(0.75,0.515625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.301703,1.709302,0.230266), DirectX::XMFLOAT2(0.8125,0.515625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.308608,1.833943,0.236748), DirectX::XMFLOAT2(0.8125,0.484375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.370982,1.830599,0.234595), DirectX::XMFLOAT2(0.75,0.515625), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.364076,1.705958,0.228112), DirectX::XMFLOAT2(0.75,0.546875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.363087,1.844014,-0.01492), DirectX::XMFLOAT2(0.8125,0.515625), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.364076,1.705958,0.228112), DirectX::XMFLOAT2(0.75,0.546875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.356182,1.719373,-0.021403), DirectX::XMFLOAT2(0.8125,0.546875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.363087,1.844014,-0.01492), DirectX::XMFLOAT2(0.8125,0.515625), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.370982,1.830599,0.234595), DirectX::XMFLOAT2(0.421875,0.859375), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.363087,1.844014,-0.01492), DirectX::XMFLOAT2(0.421875,0.796875), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.308608,1.833943,0.236748), DirectX::XMFLOAT2(0.40625,0.859375), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },
		{ DirectX::XMFLOAT3(0.363087,1.844014,-0.01492), DirectX::XMFLOAT2(0.421875,0.796875), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.300714,1.847358,-0.012767), DirectX::XMFLOAT2(0.40625,0.796875), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.308608,1.833943,0.236748), DirectX::XMFLOAT2(0.40625,0.859375), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },
		{ DirectX::XMFLOAT3(0.356182,1.719373,-0.021403), DirectX::XMFLOAT2(0.734375,0.796875), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.364076,1.705958,0.228112), DirectX::XMFLOAT2(0.734375,0.859375), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.293808,1.722717,-0.019249), DirectX::XMFLOAT2(0.71875,0.796875), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },
		{ DirectX::XMFLOAT3(0.364076,1.705958,0.228112), DirectX::XMFLOAT2(0.734375,0.859375), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.301703,1.709302,0.230266), DirectX::XMFLOAT2(0.71875,0.859375), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.293808,1.722717,-0.019249), DirectX::XMFLOAT2(0.71875,0.796875), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },
		{ DirectX::XMFLOAT3(0.363087,1.844014,-0.01492), DirectX::XMFLOAT2(0.328125,0.8125), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.356182,1.719373,-0.021403), DirectX::XMFLOAT2(0.328125,0.84375), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.300714,1.847358,-0.012767), DirectX::XMFLOAT2(0.34375,0.8125), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },
		{ DirectX::XMFLOAT3(0.356182,1.719373,-0.021403), DirectX::XMFLOAT2(0.328125,0.84375), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.293808,1.722717,-0.019249), DirectX::XMFLOAT2(0.34375,0.84375), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.300714,1.847358,-0.012767), DirectX::XMFLOAT2(0.34375,0.8125), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },
		{ DirectX::XMFLOAT3(0.308608,1.833943,0.236748), DirectX::XMFLOAT2(0.3125,0.8125), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.301703,1.709302,0.230266), DirectX::XMFLOAT2(0.3125,0.84375), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.370982,1.830599,0.234595), DirectX::XMFLOAT2(0.328125,0.8125), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },
		{ DirectX::XMFLOAT3(0.301703,1.709302,0.230266), DirectX::XMFLOAT2(0.3125,0.84375), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.364076,1.705958,0.228112), DirectX::XMFLOAT2(0.328125,0.84375), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.370982,1.830599,0.234595), DirectX::XMFLOAT2(0.328125,0.8125), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },
		{ DirectX::XMFLOAT3(0.550207,1.833983,-0.02138), DirectX::XMFLOAT2(0.75,0.65625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.543302,1.709342,-0.027862), DirectX::XMFLOAT2(0.75,0.6875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.558102,1.820568,0.228135), DirectX::XMFLOAT2(0.8125,0.65625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.543302,1.709342,-0.027862), DirectX::XMFLOAT2(0.75,0.6875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.551196,1.695928,0.221653), DirectX::XMFLOAT2(0.8125,0.6875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.558102,1.820568,0.228135), DirectX::XMFLOAT2(0.8125,0.65625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.620475,1.817225,0.225982), DirectX::XMFLOAT2(0.765625,0.6875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.613569,1.692584,0.219499), DirectX::XMFLOAT2(0.765625,0.71875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.61258,1.83064,-0.023533), DirectX::XMFLOAT2(0.828125,0.6875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.613569,1.692584,0.219499), DirectX::XMFLOAT2(0.765625,0.71875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.605675,1.705999,-0.030016), DirectX::XMFLOAT2(0.828125,0.71875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.61258,1.83064,-0.023533), DirectX::XMFLOAT2(0.828125,0.6875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.620475,1.817225,0.225982), DirectX::XMFLOAT2(0.109375,0.875), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.61258,1.83064,-0.023533), DirectX::XMFLOAT2(0.109375,0.8125), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.558102,1.820568,0.228135), DirectX::XMFLOAT2(0.09375,0.875), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },
		{ DirectX::XMFLOAT3(0.61258,1.83064,-0.023533), DirectX::XMFLOAT2(0.109375,0.8125), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.550207,1.833983,-0.02138), DirectX::XMFLOAT2(0.09375,0.8125), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.558102,1.820568,0.228135), DirectX::XMFLOAT2(0.09375,0.875), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },
		{ DirectX::XMFLOAT3(0.605675,1.705999,-0.030016), DirectX::XMFLOAT2(0.125,0.8125), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.613569,1.692584,0.219499), DirectX::XMFLOAT2(0.125,0.875), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.543302,1.709342,-0.027862), DirectX::XMFLOAT2(0.109375,0.8125), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },
		{ DirectX::XMFLOAT3(0.613569,1.692584,0.219499), DirectX::XMFLOAT2(0.125,0.875), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.551196,1.695928,0.221653), DirectX::XMFLOAT2(0.109375,0.875), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.543302,1.709342,-0.027862), DirectX::XMFLOAT2(0.109375,0.8125), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },
		{ DirectX::XMFLOAT3(0.61258,1.83064,-0.023533), DirectX::XMFLOAT2(0.359375,0.8125), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.605675,1.705999,-0.030016), DirectX::XMFLOAT2(0.359375,0.84375), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.550207,1.833983,-0.02138), DirectX::XMFLOAT2(0.375,0.8125), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },
		{ DirectX::XMFLOAT3(0.605675,1.705999,-0.030016), DirectX::XMFLOAT2(0.359375,0.84375), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.543302,1.709342,-0.027862), DirectX::XMFLOAT2(0.375,0.84375), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.550207,1.833983,-0.02138), DirectX::XMFLOAT2(0.375,0.8125), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },
		{ DirectX::XMFLOAT3(0.558102,1.820568,0.228135), DirectX::XMFLOAT2(0.34375,0.8125), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.551196,1.695928,0.221653), DirectX::XMFLOAT2(0.34375,0.84375), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.620475,1.817225,0.225982), DirectX::XMFLOAT2(0.359375,0.8125), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },
		{ DirectX::XMFLOAT3(0.551196,1.695928,0.221653), DirectX::XMFLOAT2(0.34375,0.84375), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.613569,1.692584,0.219499), DirectX::XMFLOAT2(0.359375,0.84375), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.620475,1.817225,0.225982), DirectX::XMFLOAT2(0.359375,0.8125), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },
		{ DirectX::XMFLOAT3(0.32857,1.834298,-0.092627), DirectX::XMFLOAT2(0.625,0.53125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.318212,1.647337,-0.102351), DirectX::XMFLOAT2(0.625,0.578125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.340412,1.814175,0.281645), DirectX::XMFLOAT2(0.71875,0.53125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.318212,1.647337,-0.102351), DirectX::XMFLOAT2(0.625,0.578125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.330054,1.627215,0.271921), DirectX::XMFLOAT2(0.71875,0.578125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.340412,1.814175,0.281645), DirectX::XMFLOAT2(0.71875,0.53125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.589905,1.800801,0.273032), DirectX::XMFLOAT2(0.65625,0.65625), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.579547,1.61384,0.263308), DirectX::XMFLOAT2(0.65625,0.703125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.578063,1.820923,-0.10124), DirectX::XMFLOAT2(0.75,0.65625), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.579547,1.61384,0.263308), DirectX::XMFLOAT2(0.65625,0.703125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.567705,1.633962,-0.110964), DirectX::XMFLOAT2(0.75,0.703125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.578063,1.820923,-0.10124), DirectX::XMFLOAT2(0.75,0.65625), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.589905,1.800801,0.273032), DirectX::XMFLOAT2(0.625,0.5), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.578063,1.820923,-0.10124), DirectX::XMFLOAT2(0.625,0.40625), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.340412,1.814175,0.281645), DirectX::XMFLOAT2(0.5625,0.5), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },
		{ DirectX::XMFLOAT3(0.578063,1.820923,-0.10124), DirectX::XMFLOAT2(0.625,0.40625), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.32857,1.834298,-0.092627), DirectX::XMFLOAT2(0.5625,0.40625), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },{ DirectX::XMFLOAT3(0.340412,1.814175,0.281645), DirectX::XMFLOAT2(0.5625,0.5), DirectX::XMFLOAT3(0.0552,0.9971,0.0519) },
		{ DirectX::XMFLOAT3(0.567705,1.633962,-0.110964), DirectX::XMFLOAT2(0.625,0.5), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.579547,1.61384,0.263308), DirectX::XMFLOAT2(0.625,0.59375), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.318212,1.647337,-0.102351), DirectX::XMFLOAT2(0.5625,0.5), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },
		{ DirectX::XMFLOAT3(0.579547,1.61384,0.263308), DirectX::XMFLOAT2(0.625,0.59375), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.330054,1.627215,0.271921), DirectX::XMFLOAT2(0.5625,0.59375), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },{ DirectX::XMFLOAT3(0.318212,1.647337,-0.102351), DirectX::XMFLOAT2(0.5625,0.5), DirectX::XMFLOAT3(-0.0552,-0.9971,-0.0519) },
		{ DirectX::XMFLOAT3(0.578063,1.820923,-0.10124), DirectX::XMFLOAT2(0.71875,0.609375), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.567705,1.633962,-0.110964), DirectX::XMFLOAT2(0.71875,0.65625), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.32857,1.834298,-0.092627), DirectX::XMFLOAT2(0.78125,0.609375), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },
		{ DirectX::XMFLOAT3(0.567705,1.633962,-0.110964), DirectX::XMFLOAT2(0.71875,0.65625), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.318212,1.647337,-0.102351), DirectX::XMFLOAT2(0.78125,0.65625), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },{ DirectX::XMFLOAT3(0.32857,1.834298,-0.092627), DirectX::XMFLOAT2(0.78125,0.609375), DirectX::XMFLOAT3(-0.0316,0.0537,-0.9981) },
		{ DirectX::XMFLOAT3(0.340412,1.814175,0.281645), DirectX::XMFLOAT2(0.71875,0.5625), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.330054,1.627215,0.271921), DirectX::XMFLOAT2(0.71875,0.609375), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.589905,1.800801,0.273032), DirectX::XMFLOAT2(0.78125,0.5625), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },
		{ DirectX::XMFLOAT3(0.330054,1.627215,0.271921), DirectX::XMFLOAT2(0.71875,0.609375), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.579547,1.61384,0.263308), DirectX::XMFLOAT2(0.78125,0.609375), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },{ DirectX::XMFLOAT3(0.589905,1.800801,0.273032), DirectX::XMFLOAT2(0.78125,0.5625), DirectX::XMFLOAT3(0.0316,-0.0537,0.9981) },
		{ DirectX::XMFLOAT3(0.321172,1.642306,-0.008783), DirectX::XMFLOAT2(0.78125,0.640625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.317488,1.580988,-0.020304), DirectX::XMFLOAT2(0.78125,0.65625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.327157,1.595809,0.236782), DirectX::XMFLOAT2(0.84375,0.640625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.317488,1.580988,-0.020304), DirectX::XMFLOAT2(0.78125,0.65625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.323473,1.534491,0.225261), DirectX::XMFLOAT2(0.84375,0.65625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.327157,1.595809,0.236782), DirectX::XMFLOAT2(0.84375,0.640625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.576651,1.582435,0.228169), DirectX::XMFLOAT2(0.796875,0.234375), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0344) },{ DirectX::XMFLOAT3(0.572966,1.521116,0.216648), DirectX::XMFLOAT2(0.796875,0.25), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0344) },{ DirectX::XMFLOAT3(0.570666,1.628932,-0.017396), DirectX::XMFLOAT2(0.859375,0.234375), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0344) },
		{ DirectX::XMFLOAT3(0.572966,1.521116,0.216648), DirectX::XMFLOAT2(0.796875,0.25), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.566981,1.567613,-0.028917), DirectX::XMFLOAT2(0.859375,0.25), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.570666,1.628932,-0.017396), DirectX::XMFLOAT2(0.859375,0.234375), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.576651,1.582435,0.228169), DirectX::XMFLOAT2(0.734375,0.21875), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.570666,1.628932,-0.017396), DirectX::XMFLOAT2(0.734375,0.15625), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.327157,1.595809,0.236782), DirectX::XMFLOAT2(0.671875,0.21875), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },
		{ DirectX::XMFLOAT3(0.570666,1.628932,-0.017396), DirectX::XMFLOAT2(0.734375,0.15625), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.321172,1.642306,-0.008783), DirectX::XMFLOAT2(0.671875,0.15625), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.327157,1.595809,0.236782), DirectX::XMFLOAT2(0.671875,0.21875), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },
		{ DirectX::XMFLOAT3(0.566981,1.567613,-0.028917), DirectX::XMFLOAT2(0.734375,0.21875), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.572966,1.521116,0.216648), DirectX::XMFLOAT2(0.734375,0.28125), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.317488,1.580988,-0.020304), DirectX::XMFLOAT2(0.671875,0.21875), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },
		{ DirectX::XMFLOAT3(0.572966,1.521116,0.216648), DirectX::XMFLOAT2(0.734375,0.28125), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.323473,1.534491,0.225261), DirectX::XMFLOAT2(0.671875,0.28125), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.317488,1.580988,-0.020304), DirectX::XMFLOAT2(0.671875,0.21875), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },
		{ DirectX::XMFLOAT3(0.570666,1.628932,-0.017396), DirectX::XMFLOAT2(0.796875,0.21875), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.566981,1.567613,-0.028917), DirectX::XMFLOAT2(0.796875,0.234375), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.321172,1.642306,-0.008783), DirectX::XMFLOAT2(0.859375,0.21875), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },
		{ DirectX::XMFLOAT3(0.566981,1.567613,-0.028917), DirectX::XMFLOAT2(0.796875,0.234375), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.317488,1.580988,-0.020304), DirectX::XMFLOAT2(0.859375,0.234375), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.321172,1.642306,-0.008783), DirectX::XMFLOAT2(0.859375,0.21875), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },
		{ DirectX::XMFLOAT3(0.327157,1.595809,0.236782), DirectX::XMFLOAT2(0.78125,0.625), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.323473,1.534491,0.225261), DirectX::XMFLOAT2(0.78125,0.640625), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.576651,1.582435,0.228169), DirectX::XMFLOAT2(0.84375,0.625), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },
		{ DirectX::XMFLOAT3(0.323473,1.534491,0.225261), DirectX::XMFLOAT2(0.78125,0.640625), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.572966,1.521116,0.216648), DirectX::XMFLOAT2(0.84375,0.640625), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.576651,1.582435,0.228169), DirectX::XMFLOAT2(0.84375,0.625), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },
		{ DirectX::XMFLOAT3(0.307768,1.716085,-0.058115), DirectX::XMFLOAT2(0.375,0.796875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.300398,1.593448,-0.081157), DirectX::XMFLOAT2(0.375,0.828125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.31076,1.692836,0.064667), DirectX::XMFLOAT2(0.40625,0.796875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.300398,1.593448,-0.081157), DirectX::XMFLOAT2(0.375,0.828125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.303391,1.570199,0.041626), DirectX::XMFLOAT2(0.40625,0.828125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.31076,1.692836,0.064667), DirectX::XMFLOAT2(0.40625,0.796875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.435507,1.686149,0.060361), DirectX::XMFLOAT2(0.625,0.796875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.428137,1.563512,0.037319), DirectX::XMFLOAT2(0.625,0.828125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.432514,1.709398,-0.062422), DirectX::XMFLOAT2(0.65625,0.796875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.428137,1.563512,0.037319), DirectX::XMFLOAT2(0.625,0.828125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.425145,1.586761,-0.085463), DirectX::XMFLOAT2(0.65625,0.828125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.432514,1.709398,-0.062422), DirectX::XMFLOAT2(0.65625,0.796875), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.435507,1.686149,0.060361), DirectX::XMFLOAT2(0.6875,0.828125), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.432514,1.709398,-0.062422), DirectX::XMFLOAT2(0.6875,0.796875), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.31076,1.692836,0.064667), DirectX::XMFLOAT2(0.65625,0.828125), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },
		{ DirectX::XMFLOAT3(0.432514,1.709398,-0.062422), DirectX::XMFLOAT2(0.6875,0.796875), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.307768,1.716085,-0.058115), DirectX::XMFLOAT2(0.65625,0.796875), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.31076,1.692836,0.064667), DirectX::XMFLOAT2(0.65625,0.828125), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },
		{ DirectX::XMFLOAT3(0.425145,1.586761,-0.085463), DirectX::XMFLOAT2(0.71875,0.796875), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.428137,1.563512,0.037319), DirectX::XMFLOAT2(0.71875,0.828125), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.300398,1.593448,-0.081157), DirectX::XMFLOAT2(0.6875,0.796875), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },
		{ DirectX::XMFLOAT3(0.428137,1.563512,0.037319), DirectX::XMFLOAT2(0.71875,0.828125), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.303391,1.570199,0.041626), DirectX::XMFLOAT2(0.6875,0.828125), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.300398,1.593448,-0.081157), DirectX::XMFLOAT2(0.6875,0.796875), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },
		{ DirectX::XMFLOAT3(0.432514,1.709398,-0.062422), DirectX::XMFLOAT2(0.59375,0.796875), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.425145,1.586761,-0.085463), DirectX::XMFLOAT2(0.59375,0.828125), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.307768,1.716085,-0.058115), DirectX::XMFLOAT2(0.625,0.796875), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },
		{ DirectX::XMFLOAT3(0.425145,1.586761,-0.085463), DirectX::XMFLOAT2(0.59375,0.828125), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.300398,1.593448,-0.081157), DirectX::XMFLOAT2(0.625,0.828125), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.307768,1.716085,-0.058115), DirectX::XMFLOAT2(0.625,0.796875), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },
		{ DirectX::XMFLOAT3(0.31076,1.692836,0.064667), DirectX::XMFLOAT2(0.796875,0.25), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.303391,1.570199,0.041626), DirectX::XMFLOAT2(0.796875,0.28125), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.435507,1.686149,0.060361), DirectX::XMFLOAT2(0.828125,0.25), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },
		{ DirectX::XMFLOAT3(0.303391,1.570199,0.041626), DirectX::XMFLOAT2(0.796875,0.28125), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.428137,1.563512,0.037319), DirectX::XMFLOAT2(0.828125,0.28125), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.435507,1.686149,0.060361), DirectX::XMFLOAT2(0.828125,0.25), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },
		{ DirectX::XMFLOAT3(0.463701,1.707726,-0.063498), DirectX::XMFLOAT2(0,0.8125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.456331,1.585089,-0.08654), DirectX::XMFLOAT2(0,0.84375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.466693,1.684477,0.059284), DirectX::XMFLOAT2(0.03125,0.8125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.456331,1.585089,-0.08654), DirectX::XMFLOAT2(0,0.84375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.459324,1.56184,0.036243), DirectX::XMFLOAT2(0.03125,0.84375), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.466693,1.684477,0.059284), DirectX::XMFLOAT2(0.03125,0.8125), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.59144,1.67779,0.054978), DirectX::XMFLOAT2(0.03125,0.8125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.584071,1.555153,0.031936), DirectX::XMFLOAT2(0.03125,0.84375), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.588448,1.701039,-0.067805), DirectX::XMFLOAT2(0.0625,0.8125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.584071,1.555153,0.031936), DirectX::XMFLOAT2(0.03125,0.84375), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.581078,1.578402,-0.090846), DirectX::XMFLOAT2(0.0625,0.84375), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.588448,1.701039,-0.067805), DirectX::XMFLOAT2(0.0625,0.8125), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.59144,1.67779,0.054978), DirectX::XMFLOAT2(0.84375,0.0625), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.588448,1.701039,-0.067805), DirectX::XMFLOAT2(0.84375,0.03125), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.466693,1.684477,0.059284), DirectX::XMFLOAT2(0.8125,0.0625), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },
		{ DirectX::XMFLOAT3(0.588448,1.701039,-0.067805), DirectX::XMFLOAT2(0.84375,0.03125), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.463701,1.707726,-0.063498), DirectX::XMFLOAT2(0.8125,0.03125), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },{ DirectX::XMFLOAT3(0.466693,1.684477,0.059284), DirectX::XMFLOAT2(0.8125,0.0625), DirectX::XMFLOAT3(0.0590,0.9811,0.1843) },
		{ DirectX::XMFLOAT3(0.581078,1.578402,-0.090846), DirectX::XMFLOAT2(0.09375,0.8125), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.584071,1.555153,0.031936), DirectX::XMFLOAT2(0.09375,0.84375), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.456331,1.585089,-0.08654), DirectX::XMFLOAT2(0.0625,0.8125), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },
		{ DirectX::XMFLOAT3(0.584071,1.555153,0.031936), DirectX::XMFLOAT2(0.09375,0.84375), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.459324,1.56184,0.036243), DirectX::XMFLOAT2(0.0625,0.84375), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },{ DirectX::XMFLOAT3(0.456331,1.585089,-0.08654), DirectX::XMFLOAT2(0.0625,0.8125), DirectX::XMFLOAT3(-0.0590,-0.9811,-0.1843) },
		{ DirectX::XMFLOAT3(0.588448,1.701039,-0.067805), DirectX::XMFLOAT2(0.8125,0), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.581078,1.578402,-0.090846), DirectX::XMFLOAT2(0.8125,0.03125), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.463701,1.707726,-0.063498), DirectX::XMFLOAT2(0.84375,0), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },
		{ DirectX::XMFLOAT3(0.581078,1.578402,-0.090846), DirectX::XMFLOAT2(0.8125,0.03125), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.456331,1.585089,-0.08654), DirectX::XMFLOAT2(0.84375,0.03125), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },{ DirectX::XMFLOAT3(0.463701,1.707726,-0.063498), DirectX::XMFLOAT2(0.84375,0), DirectX::XMFLOAT3(-0.0239,0.186,-0.9823) },
		{ DirectX::XMFLOAT3(0.466693,1.684477,0.059284), DirectX::XMFLOAT2(0.796875,0.78125), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.459324,1.56184,0.036243), DirectX::XMFLOAT2(0.796875,0.8125), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.59144,1.67779,0.054978), DirectX::XMFLOAT2(0.828125,0.78125), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },
		{ DirectX::XMFLOAT3(0.459324,1.56184,0.036243), DirectX::XMFLOAT2(0.796875,0.8125), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.584071,1.555153,0.031936), DirectX::XMFLOAT2(0.828125,0.8125), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },{ DirectX::XMFLOAT3(0.59144,1.67779,0.054978), DirectX::XMFLOAT2(0.828125,0.78125), DirectX::XMFLOAT3(0.0239,-0.186,0.9823) },
		{ DirectX::XMFLOAT3(0.306541,1.703026,-0.055715), DirectX::XMFLOAT2(0.734375,0.703125), DirectX::XMFLOAT3(-0.6640,0.7316,0.1547) },{ DirectX::XMFLOAT3(0.087905,1.535898,-0.20378), DirectX::XMFLOAT2(0.734375,0.78125), DirectX::XMFLOAT3(-0.6640,0.7316,0.1547) },{ DirectX::XMFLOAT3(0.273555,1.650171,0.052652), DirectX::XMFLOAT2(0.765625,0.703125), DirectX::XMFLOAT3(-0.6640,0.7316,0.1547) },
		{ DirectX::XMFLOAT3(0.087905,1.535898,-0.20378), DirectX::XMFLOAT2(0.734375,0.78125), DirectX::XMFLOAT3(-0.6640,0.7316,0.1547) },{ DirectX::XMFLOAT3(0.05492,1.483044,-0.095413), DirectX::XMFLOAT2(0.765625,0.78125), DirectX::XMFLOAT3(-0.6640,0.7316,0.1547) },{ DirectX::XMFLOAT3(0.273555,1.650171,0.052652), DirectX::XMFLOAT2(0.765625,0.703125), DirectX::XMFLOAT3(-0.6640,0.7316,0.1547) },
		{ DirectX::XMFLOAT3(0.315054,1.604448,0.042983), DirectX::XMFLOAT2(0.75,0.25), DirectX::XMFLOAT3(0.6640,-0.7316,-0.1547) },{ DirectX::XMFLOAT3(0.096419,1.437321,-0.105082), DirectX::XMFLOAT2(0.75,0.328125), DirectX::XMFLOAT3(0.6640,-0.7316,-0.1547) },{ DirectX::XMFLOAT3(0.34804,1.657303,-0.065384), DirectX::XMFLOAT2(0.78125,0.25), DirectX::XMFLOAT3(0.6640,-0.7316,-0.1547) },
		{ DirectX::XMFLOAT3(0.096419,1.437321,-0.105082), DirectX::XMFLOAT2(0.75,0.328125), DirectX::XMFLOAT3(0.6640,-0.7316,-0.1547) },{ DirectX::XMFLOAT3(0.129405,1.490175,-0.213449), DirectX::XMFLOAT2(0.78125,0.328125), DirectX::XMFLOAT3(0.6640,-0.7316,-0.1547) },{ DirectX::XMFLOAT3(0.34804,1.657303,-0.065384), DirectX::XMFLOAT2(0.78125,0.25), DirectX::XMFLOAT3(0.6640,-0.7316,-0.1547) },
		{ DirectX::XMFLOAT3(0.315054,1.604448,0.042983), DirectX::XMFLOAT2(0.265625,0.84375), DirectX::XMFLOAT3(0.6996,0.5348,0.4738) },{ DirectX::XMFLOAT3(0.34804,1.657303,-0.065384), DirectX::XMFLOAT2(0.265625,0.8125), DirectX::XMFLOAT3(0.6996,0.5348,0.4738) },{ DirectX::XMFLOAT3(0.273555,1.650171,0.052652), DirectX::XMFLOAT2(0.25,0.84375), DirectX::XMFLOAT3(0.6996,0.5348,0.4738) },
		{ DirectX::XMFLOAT3(0.34804,1.657303,-0.065384), DirectX::XMFLOAT2(0.265625,0.8125), DirectX::XMFLOAT3(0.6996,0.5348,0.4738) },{ DirectX::XMFLOAT3(0.306541,1.703026,-0.055715), DirectX::XMFLOAT2(0.25,0.8125), DirectX::XMFLOAT3(0.6996,0.5348,0.4738) },{ DirectX::XMFLOAT3(0.273555,1.650171,0.052652), DirectX::XMFLOAT2(0.25,0.84375), DirectX::XMFLOAT3(0.6996,0.5348,0.4738) },
		{ DirectX::XMFLOAT3(0.129405,1.490175,-0.213449), DirectX::XMFLOAT2(0.28125,0.8125), DirectX::XMFLOAT3(-0.6996,-0.5348,-0.4738) },{ DirectX::XMFLOAT3(0.096419,1.437321,-0.105082), DirectX::XMFLOAT2(0.28125,0.84375), DirectX::XMFLOAT3(-0.6996,-0.5348,-0.4738) },{ DirectX::XMFLOAT3(0.087905,1.535898,-0.20378), DirectX::XMFLOAT2(0.265625,0.8125), DirectX::XMFLOAT3(-0.6996,-0.5348,-0.4738) },
		{ DirectX::XMFLOAT3(0.096419,1.437321,-0.105082), DirectX::XMFLOAT2(0.28125,0.84375), DirectX::XMFLOAT3(-0.6996,-0.5348,-0.4738) },{ DirectX::XMFLOAT3(0.05492,1.483044,-0.095413), DirectX::XMFLOAT2(0.265625,0.84375), DirectX::XMFLOAT3(-0.6996,-0.5348,-0.4738) },{ DirectX::XMFLOAT3(0.087905,1.535898,-0.20378), DirectX::XMFLOAT2(0.265625,0.8125), DirectX::XMFLOAT3(-0.6996,-0.5348,-0.4738) },
		{ DirectX::XMFLOAT3(0.34804,1.657303,-0.065384), DirectX::XMFLOAT2(0.78125,0.21875), DirectX::XMFLOAT3(0.2639,0.4228,-0.8669) },{ DirectX::XMFLOAT3(0.129405,1.490175,-0.213449), DirectX::XMFLOAT2(0.78125,0.296875), DirectX::XMFLOAT3(0.2639,0.4228,-0.8669) },{ DirectX::XMFLOAT3(0.306541,1.703026,-0.055715), DirectX::XMFLOAT2(0.796875,0.21875), DirectX::XMFLOAT3(0.2639,0.4228,-0.8669) },
		{ DirectX::XMFLOAT3(0.129405,1.490175,-0.213449), DirectX::XMFLOAT2(0.78125,0.296875), DirectX::XMFLOAT3(0.2639,0.4228,-0.8669) },{ DirectX::XMFLOAT3(0.087905,1.535898,-0.20378), DirectX::XMFLOAT2(0.796875,0.296875), DirectX::XMFLOAT3(0.2639,0.4228,-0.8669) },{ DirectX::XMFLOAT3(0.306541,1.703026,-0.055715), DirectX::XMFLOAT2(0.796875,0.21875), DirectX::XMFLOAT3(0.2639,0.4228,-0.8669) },
		{ DirectX::XMFLOAT3(0.273555,1.650171,0.052652), DirectX::XMFLOAT2(0.546875,0.765625), DirectX::XMFLOAT3(-0.2639,-0.4228,0.8669) },{ DirectX::XMFLOAT3(0.05492,1.483044,-0.095413), DirectX::XMFLOAT2(0.546875,0.84375), DirectX::XMFLOAT3(-0.2639,-0.4228,0.8669) },{ DirectX::XMFLOAT3(0.315054,1.604448,0.042983), DirectX::XMFLOAT2(0.5625,0.765625), DirectX::XMFLOAT3(-0.2639,-0.4228,0.8669) },
		{ DirectX::XMFLOAT3(0.05492,1.483044,-0.095413), DirectX::XMFLOAT2(0.546875,0.84375), DirectX::XMFLOAT3(-0.2639,-0.4228,0.8669) },{ DirectX::XMFLOAT3(0.096419,1.437321,-0.105082), DirectX::XMFLOAT2(0.5625,0.84375), DirectX::XMFLOAT3(-0.2639,-0.4228,0.8669) },{ DirectX::XMFLOAT3(0.315054,1.604448,0.042983), DirectX::XMFLOAT2(0.5625,0.765625), DirectX::XMFLOAT3(-0.2639,-0.4228,0.8669) },
		{ DirectX::XMFLOAT3(0.533893,1.669089,-0.067681), DirectX::XMFLOAT2(0.75,0.328125), DirectX::XMFLOAT3(-0.7474,-0.6559,-0.106) },{ DirectX::XMFLOAT3(0.722731,1.480119,-0.229813), DirectX::XMFLOAT2(0.75,0.40625), DirectX::XMFLOAT3(-0.7474,-0.6559,-0.106) },{ DirectX::XMFLOAT3(0.56842,1.612615,0.038355), DirectX::XMFLOAT2(0.78125,0.328125), DirectX::XMFLOAT3(-0.7474,-0.6559,-0.106) },
		{ DirectX::XMFLOAT3(0.722731,1.480119,-0.229813), DirectX::XMFLOAT2(0.75,0.40625), DirectX::XMFLOAT3(-0.7474,-0.6559,-0.106) },{ DirectX::XMFLOAT3(0.757258,1.423645,-0.123777), DirectX::XMFLOAT2(0.78125,0.40625), DirectX::XMFLOAT3(-0.7474,-0.6559,-0.106) },{ DirectX::XMFLOAT3(0.56842,1.612615,0.038355), DirectX::XMFLOAT2(0.78125,0.328125), DirectX::XMFLOAT3(-0.7474,-0.6559,-0.106) },
		{ DirectX::XMFLOAT3(0.61513,1.65361,0.044978), DirectX::XMFLOAT2(0.75,0.40625), DirectX::XMFLOAT3(0.7474,0.6559,0.106) },{ DirectX::XMFLOAT3(0.803968,1.464639,-0.117153), DirectX::XMFLOAT2(0.75,0.484375), DirectX::XMFLOAT3(0.7474,0.6559,0.106) },{ DirectX::XMFLOAT3(0.580603,1.710083,-0.061058), DirectX::XMFLOAT2(0.78125,0.40625), DirectX::XMFLOAT3(0.7474,0.6559,0.106) },
		{ DirectX::XMFLOAT3(0.803968,1.464639,-0.117153), DirectX::XMFLOAT2(0.75,0.484375), DirectX::XMFLOAT3(0.7474,0.6559,0.106) },{ DirectX::XMFLOAT3(0.769441,1.521113,-0.223189), DirectX::XMFLOAT2(0.78125,0.484375), DirectX::XMFLOAT3(0.7474,0.6559,0.106) },{ DirectX::XMFLOAT3(0.580603,1.710083,-0.061058), DirectX::XMFLOAT2(0.78125,0.40625), DirectX::XMFLOAT3(0.7474,0.6559,0.106) },
		{ DirectX::XMFLOAT3(0.61513,1.65361,0.044978), DirectX::XMFLOAT2(0.5,0.84375), DirectX::XMFLOAT3(-0.6043,0.6047,0.5188) },{ DirectX::XMFLOAT3(0.580603,1.710083,-0.061058), DirectX::XMFLOAT2(0.5,0.8125), DirectX::XMFLOAT3(-0.6043,0.6047,0.5188) },{ DirectX::XMFLOAT3(0.56842,1.612615,0.038355), DirectX::XMFLOAT2(0.484375,0.84375), DirectX::XMFLOAT3(-0.6043,0.6047,0.5188) },
		{ DirectX::XMFLOAT3(0.580603,1.710083,-0.061058), DirectX::XMFLOAT2(0.5,0.8125), DirectX::XMFLOAT3(-0.6043,0.6047,0.5188) },{ DirectX::XMFLOAT3(0.533893,1.669089,-0.067681), DirectX::XMFLOAT2(0.484375,0.8125), DirectX::XMFLOAT3(-0.6043,0.6047,0.5188) },{ DirectX::XMFLOAT3(0.56842,1.612615,0.038355), DirectX::XMFLOAT2(0.484375,0.84375), DirectX::XMFLOAT3(-0.6043,0.6047,0.5188) },
		{ DirectX::XMFLOAT3(0.769441,1.521113,-0.223189), DirectX::XMFLOAT2(0.828125,0.484375), DirectX::XMFLOAT3(0.6043,-0.6047,-0.5188) },{ DirectX::XMFLOAT3(0.803968,1.464639,-0.117153), DirectX::XMFLOAT2(0.828125,0.515625), DirectX::XMFLOAT3(0.6043,-0.6047,-0.5188) },{ DirectX::XMFLOAT3(0.722731,1.480119,-0.229813), DirectX::XMFLOAT2(0.8125,0.484375), DirectX::XMFLOAT3(0.6043,-0.6047,-0.5188) },
		{ DirectX::XMFLOAT3(0.803968,1.464639,-0.117153), DirectX::XMFLOAT2(0.828125,0.515625), DirectX::XMFLOAT3(0.6043,-0.6047,-0.5188) },{ DirectX::XMFLOAT3(0.757258,1.423645,-0.123777), DirectX::XMFLOAT2(0.8125,0.515625), DirectX::XMFLOAT3(0.6043,-0.6047,-0.5188) },{ DirectX::XMFLOAT3(0.722731,1.480119,-0.229813), DirectX::XMFLOAT2(0.8125,0.484375), DirectX::XMFLOAT3(0.6043,-0.6047,-0.5188) },
		{ DirectX::XMFLOAT3(0.580603,1.710083,-0.061058), DirectX::XMFLOAT2(0.296875,0.78125), DirectX::XMFLOAT3(-0.2762,0.4518,-0.8483) },{ DirectX::XMFLOAT3(0.769441,1.521113,-0.223189), DirectX::XMFLOAT2(0.296875,0.859375), DirectX::XMFLOAT3(-0.2762,0.4518,-0.8483) },{ DirectX::XMFLOAT3(0.533893,1.669089,-0.067681), DirectX::XMFLOAT2(0.3125,0.78125), DirectX::XMFLOAT3(-0.2762,0.4518,-0.8483) },
		{ DirectX::XMFLOAT3(0.769441,1.521113,-0.223189), DirectX::XMFLOAT2(0.296875,0.859375), DirectX::XMFLOAT3(-0.2762,0.4518,-0.8483) },{ DirectX::XMFLOAT3(0.722731,1.480119,-0.229813), DirectX::XMFLOAT2(0.3125,0.859375), DirectX::XMFLOAT3(-0.2762,0.4518,-0.8483) },{ DirectX::XMFLOAT3(0.533893,1.669089,-0.067681), DirectX::XMFLOAT2(0.3125,0.78125), DirectX::XMFLOAT3(-0.2762,0.4518,-0.8483) },
		{ DirectX::XMFLOAT3(0.56842,1.612615,0.038355), DirectX::XMFLOAT2(0.28125,0.78125), DirectX::XMFLOAT3(0.2762,-0.4518,0.8483) },{ DirectX::XMFLOAT3(0.757258,1.423645,-0.123777), DirectX::XMFLOAT2(0.28125,0.859375), DirectX::XMFLOAT3(0.2762,-0.4518,0.8483) },{ DirectX::XMFLOAT3(0.61513,1.65361,0.044978), DirectX::XMFLOAT2(0.296875,0.78125), DirectX::XMFLOAT3(0.2762,-0.4518,0.8483) },
		{ DirectX::XMFLOAT3(0.757258,1.423645,-0.123777), DirectX::XMFLOAT2(0.28125,0.859375), DirectX::XMFLOAT3(0.2762,-0.4518,0.8483) },{ DirectX::XMFLOAT3(0.803968,1.464639,-0.117153), DirectX::XMFLOAT2(0.296875,0.859375), DirectX::XMFLOAT3(0.2762,-0.4518,0.8483) },{ DirectX::XMFLOAT3(0.61513,1.65361,0.044978), DirectX::XMFLOAT2(0.296875,0.78125), DirectX::XMFLOAT3(0.2762,-0.4518,0.8483) },
		{ DirectX::XMFLOAT3(0.734209,2.183668,-0.262769), DirectX::XMFLOAT2(0.640625,0.15625), DirectX::XMFLOAT3(-0.9009,0.431,0.0517) },{ DirectX::XMFLOAT3(0.516013,1.704013,-0.065975), DirectX::XMFLOAT2(0.640625,0.296875), DirectX::XMFLOAT3(-0.9009,0.431,0.0517) },{ DirectX::XMFLOAT3(0.758566,2.220559,-0.145847), DirectX::XMFLOAT2(0.671875,0.15625), DirectX::XMFLOAT3(-0.9009,0.431,0.0517) },
		{ DirectX::XMFLOAT3(0.516013,1.704013,-0.065975), DirectX::XMFLOAT2(0.640625,0.296875), DirectX::XMFLOAT3(-0.9009,0.431,0.0517) },{ DirectX::XMFLOAT3(0.54037,1.740904,0.050947), DirectX::XMFLOAT2(0.671875,0.296875), DirectX::XMFLOAT3(-0.9009,0.431,0.0517) },{ DirectX::XMFLOAT3(0.758566,2.220559,-0.145847), DirectX::XMFLOAT2(0.671875,0.15625), DirectX::XMFLOAT3(-0.9009,0.431,0.0517) },
		{ DirectX::XMFLOAT3(0.81487,2.193621,-0.149077), DirectX::XMFLOAT2(0.65625,0.375), DirectX::XMFLOAT3(0.9009,-0.431,-0.0517) },{ DirectX::XMFLOAT3(0.596674,1.713966,0.047717), DirectX::XMFLOAT2(0.65625,0.515625), DirectX::XMFLOAT3(0.9009,-0.431,-0.0517) },{ DirectX::XMFLOAT3(0.790514,2.15673,-0.265999), DirectX::XMFLOAT2(0.6875,0.375), DirectX::XMFLOAT3(0.9009,-0.431,-0.0517) },
		{ DirectX::XMFLOAT3(0.596674,1.713966,0.047717), DirectX::XMFLOAT2(0.65625,0.515625), DirectX::XMFLOAT3(0.9009,-0.431,-0.0517) },{ DirectX::XMFLOAT3(0.572317,1.677075,-0.069205), DirectX::XMFLOAT2(0.6875,0.515625), DirectX::XMFLOAT3(0.9009,-0.431,-0.0517) },{ DirectX::XMFLOAT3(0.790514,2.15673,-0.265999), DirectX::XMFLOAT2(0.6875,0.375), DirectX::XMFLOAT3(0.9009,-0.431,-0.0517) },
		{ DirectX::XMFLOAT3(0.81487,2.193621,-0.149077), DirectX::XMFLOAT2(0.4375,0.84375), DirectX::XMFLOAT3(0.3879,0.8527,-0.3499) },{ DirectX::XMFLOAT3(0.790514,2.15673,-0.265999), DirectX::XMFLOAT2(0.4375,0.8125), DirectX::XMFLOAT3(0.3879,0.8527,-0.3499) },{ DirectX::XMFLOAT3(0.758566,2.220559,-0.145847), DirectX::XMFLOAT2(0.421875,0.84375), DirectX::XMFLOAT3(0.3879,0.8527,-0.3499) },
		{ DirectX::XMFLOAT3(0.790514,2.15673,-0.265999), DirectX::XMFLOAT2(0.4375,0.8125), DirectX::XMFLOAT3(0.3879,0.8527,-0.3499) },{ DirectX::XMFLOAT3(0.734209,2.183668,-0.262769), DirectX::XMFLOAT2(0.421875,0.8125), DirectX::XMFLOAT3(0.3879,0.8527,-0.3499) },{ DirectX::XMFLOAT3(0.758566,2.220559,-0.145847), DirectX::XMFLOAT2(0.421875,0.84375), DirectX::XMFLOAT3(0.3879,0.8527,-0.3499) },
		{ DirectX::XMFLOAT3(0.572317,1.677075,-0.069205), DirectX::XMFLOAT2(0.453125,0.8125), DirectX::XMFLOAT3(-0.3879,-0.8527,0.3499) },{ DirectX::XMFLOAT3(0.596674,1.713966,0.047717), DirectX::XMFLOAT2(0.453125,0.84375), DirectX::XMFLOAT3(-0.3879,-0.8527,0.3499) },{ DirectX::XMFLOAT3(0.516013,1.704013,-0.065975), DirectX::XMFLOAT2(0.4375,0.8125), DirectX::XMFLOAT3(-0.3879,-0.8527,0.3499) },
		{ DirectX::XMFLOAT3(0.596674,1.713966,0.047717), DirectX::XMFLOAT2(0.453125,0.84375), DirectX::XMFLOAT3(-0.3879,-0.8527,0.3499) },{ DirectX::XMFLOAT3(0.54037,1.740904,0.050947), DirectX::XMFLOAT2(0.4375,0.84375), DirectX::XMFLOAT3(-0.3879,-0.8527,0.3499) },{ DirectX::XMFLOAT3(0.516013,1.704013,-0.065975), DirectX::XMFLOAT2(0.4375,0.8125), DirectX::XMFLOAT3(-0.3879,-0.8527,0.3499) },
		{ DirectX::XMFLOAT3(0.790514,2.15673,-0.265999), DirectX::XMFLOAT2(0.5625,0.75), DirectX::XMFLOAT3(-0.1949,-0.2951,-0.9354) },{ DirectX::XMFLOAT3(0.572317,1.677075,-0.069205), DirectX::XMFLOAT2(0.5625,0.890625), DirectX::XMFLOAT3(-0.1949,-0.2951,-0.9354) },{ DirectX::XMFLOAT3(0.734209,2.183668,-0.262769), DirectX::XMFLOAT2(0.578125,0.75), DirectX::XMFLOAT3(-0.1949,-0.2951,-0.9354) },
		{ DirectX::XMFLOAT3(0.572317,1.677075,-0.069205), DirectX::XMFLOAT2(0.5625,0.890625), DirectX::XMFLOAT3(-0.1949,-0.2951,-0.9354) },{ DirectX::XMFLOAT3(0.516013,1.704013,-0.065975), DirectX::XMFLOAT2(0.578125,0.890625), DirectX::XMFLOAT3(-0.1949,-0.2951,-0.9354) },{ DirectX::XMFLOAT3(0.734209,2.183668,-0.262769), DirectX::XMFLOAT2(0.578125,0.75), DirectX::XMFLOAT3(-0.1949,-0.2951,-0.9354) },
		{ DirectX::XMFLOAT3(0.758566,2.220559,-0.145847), DirectX::XMFLOAT2(0.546875,0.625), DirectX::XMFLOAT3(0.1949,0.2951,0.9354) },{ DirectX::XMFLOAT3(0.54037,1.740904,0.050947), DirectX::XMFLOAT2(0.546875,0.765625), DirectX::XMFLOAT3(0.1949,0.2951,0.9354) },{ DirectX::XMFLOAT3(0.81487,2.193621,-0.149077), DirectX::XMFLOAT2(0.5625,0.625), DirectX::XMFLOAT3(0.1949,0.2951,0.9354) },
		{ DirectX::XMFLOAT3(0.54037,1.740904,0.050947), DirectX::XMFLOAT2(0.546875,0.765625), DirectX::XMFLOAT3(0.1949,0.2951,0.9354) },{ DirectX::XMFLOAT3(0.596674,1.713966,0.047717), DirectX::XMFLOAT2(0.5625,0.765625), DirectX::XMFLOAT3(0.1949,0.2951,0.9354) },{ DirectX::XMFLOAT3(0.81487,2.193621,-0.149077), DirectX::XMFLOAT2(0.5625,0.625), DirectX::XMFLOAT3(0.1949,0.2951,0.9354) },
		{ DirectX::XMFLOAT3(0.142368,2.191475,-0.243624), DirectX::XMFLOAT2(0.59375,0.65625), DirectX::XMFLOAT3(-0.9431,-0.3322,0.012) },{ DirectX::XMFLOAT3(0.321113,1.690541,-0.060533), DirectX::XMFLOAT2(0.59375,0.796875), DirectX::XMFLOAT3(-0.9431,-0.3322,0.012) },{ DirectX::XMFLOAT3(0.130187,2.230324,-0.12544), DirectX::XMFLOAT2(0.625,0.65625), DirectX::XMFLOAT3(-0.9431,-0.3322,0.012) },
		{ DirectX::XMFLOAT3(0.321113,1.690541,-0.060533), DirectX::XMFLOAT2(0.59375,0.796875), DirectX::XMFLOAT3(-0.9431,-0.3322,0.012) },{ DirectX::XMFLOAT3(0.308932,1.729391,0.057651), DirectX::XMFLOAT2(0.625,0.796875), DirectX::XMFLOAT3(-0.9431,-0.3322,0.012) },{ DirectX::XMFLOAT3(0.130187,2.230324,-0.12544), DirectX::XMFLOAT2(0.625,0.65625), DirectX::XMFLOAT3(-0.9431,-0.3322,0.012) },
		{ DirectX::XMFLOAT3(0.189134,2.251084,-0.126189), DirectX::XMFLOAT2(0.625,0.65625), DirectX::XMFLOAT3(0.9431,0.3322,-0.012) },{ DirectX::XMFLOAT3(0.367879,1.750151,0.056902), DirectX::XMFLOAT2(0.625,0.796875), DirectX::XMFLOAT3(0.9431,0.3322,-0.012) },{ DirectX::XMFLOAT3(0.201314,2.212234,-0.244373), DirectX::XMFLOAT2(0.65625,0.65625), DirectX::XMFLOAT3(0.9431,0.3322,-0.012) },
		{ DirectX::XMFLOAT3(0.367879,1.750151,0.056902), DirectX::XMFLOAT2(0.625,0.796875), DirectX::XMFLOAT3(0.9431,0.3322,-0.012) },{ DirectX::XMFLOAT3(0.380059,1.711301,-0.061282), DirectX::XMFLOAT2(0.65625,0.796875), DirectX::XMFLOAT3(0.9431,0.3322,-0.012) },{ DirectX::XMFLOAT3(0.201314,2.212234,-0.244373), DirectX::XMFLOAT2(0.65625,0.65625), DirectX::XMFLOAT3(0.9431,0.3322,-0.012) },
		{ DirectX::XMFLOAT3(0.189134,2.251084,-0.126189), DirectX::XMFLOAT2(0.46875,0.84375), DirectX::XMFLOAT3(-0.3178,0.8905,-0.3255) },{ DirectX::XMFLOAT3(0.201314,2.212234,-0.244373), DirectX::XMFLOAT2(0.46875,0.8125), DirectX::XMFLOAT3(-0.3178,0.8905,-0.3255) },{ DirectX::XMFLOAT3(0.130187,2.230324,-0.12544), DirectX::XMFLOAT2(0.453125,0.84375), DirectX::XMFLOAT3(-0.3178,0.8905,-0.3255) },
		{ DirectX::XMFLOAT3(0.201314,2.212234,-0.244373), DirectX::XMFLOAT2(0.46875,0.8125), DirectX::XMFLOAT3(-0.3178,0.8905,-0.3255) },{ DirectX::XMFLOAT3(0.142368,2.191475,-0.243624), DirectX::XMFLOAT2(0.453125,0.8125), DirectX::XMFLOAT3(-0.3178,0.8905,-0.3255) },{ DirectX::XMFLOAT3(0.130187,2.230324,-0.12544), DirectX::XMFLOAT2(0.453125,0.84375), DirectX::XMFLOAT3(-0.3178,0.8905,-0.3255) },
		{ DirectX::XMFLOAT3(0.380059,1.711301,-0.061282), DirectX::XMFLOAT2(0.484375,0.8125), DirectX::XMFLOAT3(0.3178,-0.8905,0.3255) },{ DirectX::XMFLOAT3(0.367879,1.750151,0.056902), DirectX::XMFLOAT2(0.484375,0.84375), DirectX::XMFLOAT3(0.3178,-0.8905,0.3255) },{ DirectX::XMFLOAT3(0.321113,1.690541,-0.060533), DirectX::XMFLOAT2(0.46875,0.8125), DirectX::XMFLOAT3(0.3178,-0.8905,0.3255) },
		{ DirectX::XMFLOAT3(0.367879,1.750151,0.056902), DirectX::XMFLOAT2(0.484375,0.84375), DirectX::XMFLOAT3(0.3178,-0.8905,0.3255) },{ DirectX::XMFLOAT3(0.308932,1.729391,0.057651), DirectX::XMFLOAT2(0.46875,0.84375), DirectX::XMFLOAT3(0.3178,-0.8905,0.3255) },{ DirectX::XMFLOAT3(0.321113,1.690541,-0.060533), DirectX::XMFLOAT2(0.46875,0.8125), DirectX::XMFLOAT3(0.3178,-0.8905,0.3255) },
		{ DirectX::XMFLOAT3(0.201314,2.212234,-0.244373), DirectX::XMFLOAT2(0.765625,0.09375), DirectX::XMFLOAT3(0.0974,-0.3108,-0.9455) },{ DirectX::XMFLOAT3(0.380059,1.711301,-0.061282), DirectX::XMFLOAT2(0.765625,0.234375), DirectX::XMFLOAT3(0.0974,-0.3108,-0.9455) },{ DirectX::XMFLOAT3(0.142368,2.191475,-0.243624), DirectX::XMFLOAT2(0.78125,0.09375), DirectX::XMFLOAT3(0.0974,-0.3108,-0.9455) },
		{ DirectX::XMFLOAT3(0.380059,1.711301,-0.061282), DirectX::XMFLOAT2(0.765625,0.234375), DirectX::XMFLOAT3(0.0974,-0.3108,-0.9455) },{ DirectX::XMFLOAT3(0.321113,1.690541,-0.060533), DirectX::XMFLOAT2(0.78125,0.234375), DirectX::XMFLOAT3(0.0974,-0.3108,-0.9455) },{ DirectX::XMFLOAT3(0.142368,2.191475,-0.243624), DirectX::XMFLOAT2(0.78125,0.09375), DirectX::XMFLOAT3(0.0974,-0.3108,-0.9455) },
		{ DirectX::XMFLOAT3(0.130187,2.230324,-0.12544), DirectX::XMFLOAT2(0.578125,0.75), DirectX::XMFLOAT3(-0.0974,0.3108,0.9455) },{ DirectX::XMFLOAT3(0.308932,1.729391,0.057651), DirectX::XMFLOAT2(0.578125,0.890625), DirectX::XMFLOAT3(-0.0974,0.3108,0.9455) },{ DirectX::XMFLOAT3(0.189134,2.251084,-0.126189), DirectX::XMFLOAT2(0.59375,0.75), DirectX::XMFLOAT3(-0.0974,0.3108,0.9455) },
		{ DirectX::XMFLOAT3(0.308932,1.729391,0.057651), DirectX::XMFLOAT2(0.578125,0.890625), DirectX::XMFLOAT3(-0.0974,0.3108,0.9455) },{ DirectX::XMFLOAT3(0.367879,1.750151,0.056902), DirectX::XMFLOAT2(0.59375,0.890625), DirectX::XMFLOAT3(-0.0974,0.3108,0.9455) },{ DirectX::XMFLOAT3(0.189134,2.251084,-0.126189), DirectX::XMFLOAT2(0.59375,0.75), DirectX::XMFLOAT3(-0.0974,0.3108,0.9455) },
		{ DirectX::XMFLOAT3(0.314756,1.036515,-0.161039), DirectX::XMFLOAT2(0.25,0.5625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.304452,0.573815,0.258993), DirectX::XMFLOAT2(0.25,0.71875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.322438,1.120281,-0.068576), DirectX::XMFLOAT2(0.28125,0.5625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.304452,0.573815,0.258993), DirectX::XMFLOAT2(0.25,0.71875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.312135,0.65758,0.351456), DirectX::XMFLOAT2(0.28125,0.71875), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },{ DirectX::XMFLOAT3(0.322438,1.120281,-0.068576), DirectX::XMFLOAT2(0.28125,0.5625), DirectX::XMFLOAT3(-0.9980,0.0535,0.0345) },
		{ DirectX::XMFLOAT3(0.509558,1.11025,-0.075036), DirectX::XMFLOAT2(0.5625,0.59375), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.499255,0.647549,0.344996), DirectX::XMFLOAT2(0.5625,0.75), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.501876,1.026485,-0.167499), DirectX::XMFLOAT2(0.59375,0.59375), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.499255,0.647549,0.344996), DirectX::XMFLOAT2(0.5625,0.75), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.491572,0.563784,0.252534), DirectX::XMFLOAT2(0.59375,0.75), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },{ DirectX::XMFLOAT3(0.501876,1.026485,-0.167499), DirectX::XMFLOAT2(0.59375,0.59375), DirectX::XMFLOAT3(0.9980,-0.0535,-0.0345) },
		{ DirectX::XMFLOAT3(0.509558,1.11025,-0.075036), DirectX::XMFLOAT2(0.109375,0.8125), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },{ DirectX::XMFLOAT3(0.501876,1.026485,-0.167499), DirectX::XMFLOAT2(0.109375,0.78125), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },{ DirectX::XMFLOAT3(0.322438,1.120281,-0.068576), DirectX::XMFLOAT2(0.0625,0.8125), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },
		{ DirectX::XMFLOAT3(0.501876,1.026485,-0.167499), DirectX::XMFLOAT2(0.109375,0.78125), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },{ DirectX::XMFLOAT3(0.314756,1.036515,-0.161039), DirectX::XMFLOAT2(0.0625,0.78125), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },{ DirectX::XMFLOAT3(0.322438,1.120281,-0.068576), DirectX::XMFLOAT2(0.0625,0.8125), DirectX::XMFLOAT3(0.0165,0.7403,-0.6721) },
		{ DirectX::XMFLOAT3(0.491572,0.563784,0.252534), DirectX::XMFLOAT2(0.828125,0.09375), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },{ DirectX::XMFLOAT3(0.499255,0.647549,0.344996), DirectX::XMFLOAT2(0.828125,0.125), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },{ DirectX::XMFLOAT3(0.304452,0.573815,0.258993), DirectX::XMFLOAT2(0.78125,0.09375), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },
		{ DirectX::XMFLOAT3(0.499255,0.647549,0.344996), DirectX::XMFLOAT2(0.828125,0.125), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },{ DirectX::XMFLOAT3(0.312135,0.65758,0.351456), DirectX::XMFLOAT2(0.78125,0.125), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },{ DirectX::XMFLOAT3(0.304452,0.573815,0.258993), DirectX::XMFLOAT2(0.78125,0.09375), DirectX::XMFLOAT3(-0.0165,-0.7403,0.6721) },
		{ DirectX::XMFLOAT3(0.501876,1.026485,-0.167499), DirectX::XMFLOAT2(0.5625,0.09375), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },{ DirectX::XMFLOAT3(0.491572,0.563784,0.252534), DirectX::XMFLOAT2(0.5625,0.25), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },{ DirectX::XMFLOAT3(0.314756,1.036515,-0.161039), DirectX::XMFLOAT2(0.609375,0.09375), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },
		{ DirectX::XMFLOAT3(0.491572,0.563784,0.252534), DirectX::XMFLOAT2(0.5625,0.25), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },{ DirectX::XMFLOAT3(0.304452,0.573815,0.258993), DirectX::XMFLOAT2(0.609375,0.25), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },{ DirectX::XMFLOAT3(0.314756,1.036515,-0.161039), DirectX::XMFLOAT2(0.609375,0.09375), DirectX::XMFLOAT3(-0.0615,-0.6701,-0.7397) },
		{ DirectX::XMFLOAT3(0.322438,1.120281,-0.068576), DirectX::XMFLOAT2(0.0625,0.5625), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },{ DirectX::XMFLOAT3(0.312135,0.65758,0.351456), DirectX::XMFLOAT2(0.0625,0.71875), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },{ DirectX::XMFLOAT3(0.509558,1.11025,-0.075036), DirectX::XMFLOAT2(0.109375,0.5625), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },
		{ DirectX::XMFLOAT3(0.312135,0.65758,0.351456), DirectX::XMFLOAT2(0.0625,0.71875), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },{ DirectX::XMFLOAT3(0.499255,0.647549,0.344996), DirectX::XMFLOAT2(0.109375,0.71875), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },{ DirectX::XMFLOAT3(0.509558,1.11025,-0.075036), DirectX::XMFLOAT2(0.109375,0.5625), DirectX::XMFLOAT3(0.0615,0.6701,0.7397) },
		{ DirectX::XMFLOAT3(0.272773,0.750675,-0.008368), DirectX::XMFLOAT2(0.3125,0.46875), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(-0.123658,0.543574,0.428182), DirectX::XMFLOAT2(0.3125,0.625), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(0.299992,0.964723,0.117895), DirectX::XMFLOAT2(0.375,0.46875), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },
		{ DirectX::XMFLOAT3(-0.123658,0.543574,0.428182), DirectX::XMFLOAT2(0.3125,0.625), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(-0.09644,0.757622,0.554445), DirectX::XMFLOAT2(0.375,0.625), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(0.299992,0.964723,0.117895), DirectX::XMFLOAT2(0.375,0.46875), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },
		{ DirectX::XMFLOAT3(0.539176,0.84085,0.276332), DirectX::XMFLOAT2(0.375,0.46875), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.142745,0.633749,0.712882), DirectX::XMFLOAT2(0.375,0.625), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.511958,0.626801,0.150069), DirectX::XMFLOAT2(0.4375,0.46875), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },
		{ DirectX::XMFLOAT3(0.142745,0.633749,0.712882), DirectX::XMFLOAT2(0.375,0.625), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.115526,0.4197,0.586619), DirectX::XMFLOAT2(0.4375,0.625), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.511958,0.626801,0.150069), DirectX::XMFLOAT2(0.4375,0.46875), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },
		{ DirectX::XMFLOAT3(0.539176,0.84085,0.276332), DirectX::XMFLOAT2(0.671875,0.65625), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(0.511958,0.626801,0.150069), DirectX::XMFLOAT2(0.671875,0.59375), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(0.299992,0.964723,0.117895), DirectX::XMFLOAT2(0.59375,0.65625), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },
		{ DirectX::XMFLOAT3(0.511958,0.626801,0.150069), DirectX::XMFLOAT2(0.671875,0.59375), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(0.272773,0.750675,-0.008368), DirectX::XMFLOAT2(0.59375,0.59375), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(0.299992,0.964723,0.117895), DirectX::XMFLOAT2(0.59375,0.65625), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },
		{ DirectX::XMFLOAT3(0.115526,0.4197,0.586619), DirectX::XMFLOAT2(0.6875,0.09375), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(0.142745,0.633749,0.712882), DirectX::XMFLOAT2(0.6875,0.15625), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(-0.123658,0.543574,0.428182), DirectX::XMFLOAT2(0.609375,0.09375), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },
		{ DirectX::XMFLOAT3(0.142745,0.633749,0.712882), DirectX::XMFLOAT2(0.6875,0.15625), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(-0.09644,0.757622,0.554445), DirectX::XMFLOAT2(0.609375,0.15625), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(-0.123658,0.543574,0.428182), DirectX::XMFLOAT2(0.609375,0.09375), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },
		{ DirectX::XMFLOAT3(0.511958,0.626801,0.150069), DirectX::XMFLOAT2(0.078125,0.375), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(0.115526,0.4197,0.586619), DirectX::XMFLOAT2(0.078125,0.53125), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(0.272773,0.750675,-0.008368), DirectX::XMFLOAT2(0.15625,0.375), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },
		{ DirectX::XMFLOAT3(0.115526,0.4197,0.586619), DirectX::XMFLOAT2(0.078125,0.53125), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(-0.123658,0.543574,0.428182), DirectX::XMFLOAT2(0.15625,0.53125), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(0.272773,0.750675,-0.008368), DirectX::XMFLOAT2(0.15625,0.375), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },
		{ DirectX::XMFLOAT3(0.299992,0.964723,0.117895), DirectX::XMFLOAT2(0,0.375), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(-0.09644,0.757622,0.554445), DirectX::XMFLOAT2(0,0.53125), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.539176,0.84085,0.276332), DirectX::XMFLOAT2(0.078125,0.375), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },
		{ DirectX::XMFLOAT3(-0.09644,0.757622,0.554445), DirectX::XMFLOAT2(0,0.53125), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.142745,0.633749,0.712882), DirectX::XMFLOAT2(0.078125,0.53125), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.539176,0.84085,0.276332), DirectX::XMFLOAT2(0.078125,0.375), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },
		{ DirectX::XMFLOAT3(0.224414,0.35102,-0.062921), DirectX::XMFLOAT2(0.28125,0.1875), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(-0.013445,0.226759,0.199009), DirectX::XMFLOAT2(0.28125,0.28125), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(0.333289,1.207214,0.44213), DirectX::XMFLOAT2(0.53125,0.1875), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },
		{ DirectX::XMFLOAT3(-0.013445,0.226759,0.199009), DirectX::XMFLOAT2(0.28125,0.28125), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(0.09543,1.082953,0.704061), DirectX::XMFLOAT2(0.53125,0.28125), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(0.333289,1.207214,0.44213), DirectX::XMFLOAT2(0.53125,0.1875), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },
		{ DirectX::XMFLOAT3(0.428963,1.157665,0.505505), DirectX::XMFLOAT2(0.28125,0.28125), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.191104,1.033404,0.767435), DirectX::XMFLOAT2(0.28125,0.375), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.320088,0.30147,0.000454), DirectX::XMFLOAT2(0.53125,0.28125), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },
		{ DirectX::XMFLOAT3(0.191104,1.033404,0.767435), DirectX::XMFLOAT2(0.28125,0.375), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.082229,0.177209,0.262384), DirectX::XMFLOAT2(0.53125,0.375), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.320088,0.30147,0.000454), DirectX::XMFLOAT2(0.53125,0.28125), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },
		{ DirectX::XMFLOAT3(0.428963,1.157665,0.505505), DirectX::XMFLOAT2(0.03125,0.8125), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(0.320088,0.30147,0.000454), DirectX::XMFLOAT2(0.03125,0.5625), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(0.333289,1.207214,0.44213), DirectX::XMFLOAT2(0,0.8125), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },
		{ DirectX::XMFLOAT3(0.320088,0.30147,0.000454), DirectX::XMFLOAT2(0.03125,0.5625), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(0.224414,0.35102,-0.062921), DirectX::XMFLOAT2(0,0.5625), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(0.333289,1.207214,0.44213), DirectX::XMFLOAT2(0,0.8125), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },
		{ DirectX::XMFLOAT3(0.082229,0.177209,0.262384), DirectX::XMFLOAT2(0.0625,0.5625), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(0.191104,1.033404,0.767435), DirectX::XMFLOAT2(0.0625,0.8125), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(-0.013445,0.226759,0.199009), DirectX::XMFLOAT2(0.03125,0.5625), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },
		{ DirectX::XMFLOAT3(0.191104,1.033404,0.767435), DirectX::XMFLOAT2(0.0625,0.8125), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(0.09543,1.082953,0.704061), DirectX::XMFLOAT2(0.03125,0.8125), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(-0.013445,0.226759,0.199009), DirectX::XMFLOAT2(0.03125,0.5625), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },
		{ DirectX::XMFLOAT3(0.320088,0.30147,0.000454), DirectX::XMFLOAT2(0.71875,0.375), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(0.082229,0.177209,0.262384), DirectX::XMFLOAT2(0.71875,0.46875), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(0.224414,0.35102,-0.062921), DirectX::XMFLOAT2(0.75,0.375), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },
		{ DirectX::XMFLOAT3(0.082229,0.177209,0.262384), DirectX::XMFLOAT2(0.71875,0.46875), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(-0.013445,0.226759,0.199009), DirectX::XMFLOAT2(0.75,0.46875), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(0.224414,0.35102,-0.062921), DirectX::XMFLOAT2(0.75,0.375), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },
		{ DirectX::XMFLOAT3(0.333289,1.207214,0.44213), DirectX::XMFLOAT2(0.71875,0.28125), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.09543,1.082953,0.704061), DirectX::XMFLOAT2(0.71875,0.375), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.428963,1.157665,0.505505), DirectX::XMFLOAT2(0.75,0.28125), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },
		{ DirectX::XMFLOAT3(0.09543,1.082953,0.704061), DirectX::XMFLOAT2(0.71875,0.375), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.191104,1.033404,0.767435), DirectX::XMFLOAT2(0.75,0.375), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.428963,1.157665,0.505505), DirectX::XMFLOAT2(0.75,0.28125), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },
		{ DirectX::XMFLOAT3(-0.110648,0.923803,-0.095461), DirectX::XMFLOAT2(0.484375,0.71875), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(-0.348507,0.799542,0.16647), DirectX::XMFLOAT2(0.484375,0.8125), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(-0.097039,1.030827,-0.032329), DirectX::XMFLOAT2(0.515625,0.71875), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },
		{ DirectX::XMFLOAT3(-0.348507,0.799542,0.16647), DirectX::XMFLOAT2(0.484375,0.8125), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(-0.334898,0.906567,0.229601), DirectX::XMFLOAT2(0.515625,0.8125), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },{ DirectX::XMFLOAT3(-0.097039,1.030827,-0.032329), DirectX::XMFLOAT2(0.515625,0.71875), DirectX::XMFLOAT3(-0.7654,0.3964,-0.507) },
		{ DirectX::XMFLOAT3(0.764025,0.584881,0.538045), DirectX::XMFLOAT2(0.515625,0.71875), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.526166,0.460621,0.799975), DirectX::XMFLOAT2(0.515625,0.8125), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.750416,0.477857,0.474913), DirectX::XMFLOAT2(0.546875,0.71875), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },
		{ DirectX::XMFLOAT3(0.526166,0.460621,0.799975), DirectX::XMFLOAT2(0.515625,0.8125), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.512557,0.353596,0.736844), DirectX::XMFLOAT2(0.546875,0.8125), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },{ DirectX::XMFLOAT3(0.750416,0.477857,0.474913), DirectX::XMFLOAT2(0.546875,0.71875), DirectX::XMFLOAT3(0.7654,-0.3964,0.507) },
		{ DirectX::XMFLOAT3(0.764025,0.584881,0.538045), DirectX::XMFLOAT2(0.8125,0.0625), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(0.750416,0.477857,0.474913), DirectX::XMFLOAT2(0.8125,0.03125), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(-0.097039,1.030827,-0.032329), DirectX::XMFLOAT2(0.53125,0.0625), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },
		{ DirectX::XMFLOAT3(0.750416,0.477857,0.474913), DirectX::XMFLOAT2(0.8125,0.03125), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(-0.110648,0.923803,-0.095461), DirectX::XMFLOAT2(0.53125,0.03125), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },{ DirectX::XMFLOAT3(-0.097039,1.030827,-0.032329), DirectX::XMFLOAT2(0.53125,0.0625), DirectX::XMFLOAT3(0.6343,0.3314,-0.6985) },
		{ DirectX::XMFLOAT3(0.512557,0.353596,0.736844), DirectX::XMFLOAT2(0.8125,0.0625), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(0.526166,0.460621,0.799975), DirectX::XMFLOAT2(0.8125,0.09375), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(-0.348507,0.799542,0.16647), DirectX::XMFLOAT2(0.53125,0.0625), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },
		{ DirectX::XMFLOAT3(0.526166,0.460621,0.799975), DirectX::XMFLOAT2(0.8125,0.09375), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(-0.334898,0.906567,0.229601), DirectX::XMFLOAT2(0.53125,0.09375), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },{ DirectX::XMFLOAT3(-0.348507,0.799542,0.16647), DirectX::XMFLOAT2(0.53125,0.0625), DirectX::XMFLOAT3(-0.6343,-0.3314,0.6985) },
		{ DirectX::XMFLOAT3(0.750416,0.477857,0.474913), DirectX::XMFLOAT2(0,0.28125), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(0.512557,0.353596,0.736844), DirectX::XMFLOAT2(0,0.375), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(-0.110648,0.923803,-0.095461), DirectX::XMFLOAT2(0.28125,0.28125), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },
		{ DirectX::XMFLOAT3(0.512557,0.353596,0.736844), DirectX::XMFLOAT2(0,0.375), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(-0.348507,0.799542,0.16647), DirectX::XMFLOAT2(0.28125,0.375), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },{ DirectX::XMFLOAT3(-0.110648,0.923803,-0.095461), DirectX::XMFLOAT2(0.28125,0.28125), DirectX::XMFLOAT3(-0.1089,-0.8562,-0.5051) },
		{ DirectX::XMFLOAT3(-0.097039,1.030827,-0.032329), DirectX::XMFLOAT2(0,0.1875), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(-0.334898,0.906567,0.229601), DirectX::XMFLOAT2(0,0.28125), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.764025,0.584881,0.538045), DirectX::XMFLOAT2(0.28125,0.1875), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },
		{ DirectX::XMFLOAT3(-0.334898,0.906567,0.229601), DirectX::XMFLOAT2(0,0.28125), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.526166,0.460621,0.799975), DirectX::XMFLOAT2(0.28125,0.28125), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },{ DirectX::XMFLOAT3(0.764025,0.584881,0.538045), DirectX::XMFLOAT2(0.28125,0.1875), DirectX::XMFLOAT3(0.1089,0.8562,0.5051) },
		{ DirectX::XMFLOAT3(0.036749,0.497366,0.510363), DirectX::XMFLOAT2(0.609375,0.15625), DirectX::XMFLOAT3(0.4816,-0.3554,-0.8011) },{ DirectX::XMFLOAT3(-0.480761,0.569207,0.167366), DirectX::XMFLOAT2(0.609375,0.3125), DirectX::XMFLOAT3(0.4816,-0.3554,-0.8011) },{ DirectX::XMFLOAT3(0.07264,0.613318,0.480498), DirectX::XMFLOAT2(0.640625,0.15625), DirectX::XMFLOAT3(0.4816,-0.3554,-0.8011) },
		{ DirectX::XMFLOAT3(-0.480761,0.569207,0.167366), DirectX::XMFLOAT2(0.609375,0.3125), DirectX::XMFLOAT3(0.4816,-0.3554,-0.8011) },{ DirectX::XMFLOAT3(-0.44487,0.685159,0.1375), DirectX::XMFLOAT2(0.640625,0.3125), DirectX::XMFLOAT3(0.4816,-0.3554,-0.8011) },{ DirectX::XMFLOAT3(0.07264,0.613318,0.480498), DirectX::XMFLOAT2(0.640625,0.15625), DirectX::XMFLOAT3(0.4816,-0.3554,-0.8011) },
		{ DirectX::XMFLOAT3(-0.017662,0.679957,0.630701), DirectX::XMFLOAT2(0.3125,0.625), DirectX::XMFLOAT3(-0.4816,0.3554,0.8011) },{ DirectX::XMFLOAT3(-0.535172,0.751798,0.287704), DirectX::XMFLOAT2(0.3125,0.78125), DirectX::XMFLOAT3(-0.4816,0.3554,0.8011) },{ DirectX::XMFLOAT3(-0.053553,0.564005,0.660567), DirectX::XMFLOAT2(0.34375,0.625), DirectX::XMFLOAT3(-0.4816,0.3554,0.8011) },
		{ DirectX::XMFLOAT3(-0.535172,0.751798,0.287704), DirectX::XMFLOAT2(0.3125,0.78125), DirectX::XMFLOAT3(-0.4816,0.3554,0.8011) },{ DirectX::XMFLOAT3(-0.571063,0.635846,0.317569), DirectX::XMFLOAT2(0.34375,0.78125), DirectX::XMFLOAT3(-0.4816,0.3554,0.8011) },{ DirectX::XMFLOAT3(-0.053553,0.564005,0.660567), DirectX::XMFLOAT2(0.34375,0.625), DirectX::XMFLOAT3(-0.4816,0.3554,0.8011) },
		{ DirectX::XMFLOAT3(-0.017662,0.679957,0.630701), DirectX::XMFLOAT2(0.15625,0.8125), DirectX::XMFLOAT3(0.8280,-0.1149,0.5488) },{ DirectX::XMFLOAT3(-0.053553,0.564005,0.660567), DirectX::XMFLOAT2(0.15625,0.78125), DirectX::XMFLOAT3(0.8280,-0.1149,0.5488) },{ DirectX::XMFLOAT3(0.07264,0.613318,0.480498), DirectX::XMFLOAT2(0.109375,0.8125), DirectX::XMFLOAT3(0.8280,-0.1149,0.5488) },
		{ DirectX::XMFLOAT3(-0.053553,0.564005,0.660567), DirectX::XMFLOAT2(0.15625,0.78125), DirectX::XMFLOAT3(0.8280,-0.1149,0.5488) },{ DirectX::XMFLOAT3(0.036749,0.497366,0.510363), DirectX::XMFLOAT2(0.109375,0.78125), DirectX::XMFLOAT3(0.8280,-0.1149,0.5488) },{ DirectX::XMFLOAT3(0.07264,0.613318,0.480498), DirectX::XMFLOAT2(0.109375,0.8125), DirectX::XMFLOAT3(0.8280,-0.1149,0.5488) },
		{ DirectX::XMFLOAT3(-0.571063,0.635846,0.317569), DirectX::XMFLOAT2(0.828125,0.125), DirectX::XMFLOAT3(-0.8280,0.1149,-0.5488) },{ DirectX::XMFLOAT3(-0.535172,0.751798,0.287704), DirectX::XMFLOAT2(0.828125,0.15625), DirectX::XMFLOAT3(-0.8280,0.1149,-0.5488) },{ DirectX::XMFLOAT3(-0.480761,0.569207,0.167366), DirectX::XMFLOAT2(0.78125,0.125), DirectX::XMFLOAT3(-0.8280,0.1149,-0.5488) },
		{ DirectX::XMFLOAT3(-0.535172,0.751798,0.287704), DirectX::XMFLOAT2(0.828125,0.15625), DirectX::XMFLOAT3(-0.8280,0.1149,-0.5488) },{ DirectX::XMFLOAT3(-0.44487,0.685159,0.1375), DirectX::XMFLOAT2(0.78125,0.15625), DirectX::XMFLOAT3(-0.8280,0.1149,-0.5488) },{ DirectX::XMFLOAT3(-0.480761,0.569207,0.167366), DirectX::XMFLOAT2(0.78125,0.125), DirectX::XMFLOAT3(-0.8280,0.1149,-0.5488) },
		{ DirectX::XMFLOAT3(-0.053553,0.564005,0.660567), DirectX::XMFLOAT2(0.15625,0.5625), DirectX::XMFLOAT3(-0.2871,-0.9276,0.2389) },{ DirectX::XMFLOAT3(-0.571063,0.635846,0.317569), DirectX::XMFLOAT2(0.15625,0.71875), DirectX::XMFLOAT3(-0.2871,-0.9276,0.2389) },{ DirectX::XMFLOAT3(0.036749,0.497366,0.510363), DirectX::XMFLOAT2(0.203125,0.5625), DirectX::XMFLOAT3(-0.2871,-0.9276,0.2389) },
		{ DirectX::XMFLOAT3(-0.571063,0.635846,0.317569), DirectX::XMFLOAT2(0.15625,0.71875), DirectX::XMFLOAT3(-0.2871,-0.9276,0.2389) },{ DirectX::XMFLOAT3(-0.480761,0.569207,0.167366), DirectX::XMFLOAT2(0.203125,0.71875), DirectX::XMFLOAT3(-0.2871,-0.9276,0.2389) },{ DirectX::XMFLOAT3(0.036749,0.497366,0.510363), DirectX::XMFLOAT2(0.203125,0.5625), DirectX::XMFLOAT3(-0.2871,-0.9276,0.2389) },
		{ DirectX::XMFLOAT3(0.07264,0.613318,0.480498), DirectX::XMFLOAT2(0.109375,0.5625), DirectX::XMFLOAT3(0.2871,0.9276,-0.2389) },{ DirectX::XMFLOAT3(-0.44487,0.685159,0.1375), DirectX::XMFLOAT2(0.109375,0.71875), DirectX::XMFLOAT3(0.2871,0.9276,-0.2389) },{ DirectX::XMFLOAT3(-0.017662,0.679957,0.630701), DirectX::XMFLOAT2(0.15625,0.5625), DirectX::XMFLOAT3(0.2871,0.9276,-0.2389) },
		{ DirectX::XMFLOAT3(-0.44487,0.685159,0.1375), DirectX::XMFLOAT2(0.109375,0.71875), DirectX::XMFLOAT3(0.2871,0.9276,-0.2389) },{ DirectX::XMFLOAT3(-0.535172,0.751798,0.287704), DirectX::XMFLOAT2(0.15625,0.71875), DirectX::XMFLOAT3(0.2871,0.9276,-0.2389) },{ DirectX::XMFLOAT3(-0.017662,0.679957,0.630701), DirectX::XMFLOAT2(0.15625,0.5625), DirectX::XMFLOAT3(0.2871,0.9276,-0.2389) },
		{ DirectX::XMFLOAT3(-0.353079,0.483528,0.237896), DirectX::XMFLOAT2(0.4375,0.46875), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },{ DirectX::XMFLOAT3(-0.464721,0.502372,-0.376763), DirectX::XMFLOAT2(0.4375,0.625), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },{ DirectX::XMFLOAT3(-0.269963,0.719175,0.230024), DirectX::XMFLOAT2(0.5,0.46875), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(-0.464721,0.502372,-0.376763), DirectX::XMFLOAT2(0.4375,0.625), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },{ DirectX::XMFLOAT3(-0.381604,0.738019,-0.384635), DirectX::XMFLOAT2(0.5,0.625), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },{ DirectX::XMFLOAT3(-0.269963,0.719175,0.230024), DirectX::XMFLOAT2(0.5,0.46875), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(-0.559352,0.82311,0.285773), DirectX::XMFLOAT2(0.5,0.46875), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },{ DirectX::XMFLOAT3(-0.670993,0.841954,-0.328887), DirectX::XMFLOAT2(0.5,0.625), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },{ DirectX::XMFLOAT3(-0.642468,0.587462,0.293645), DirectX::XMFLOAT2(0.5625,0.46875), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-0.670993,0.841954,-0.328887), DirectX::XMFLOAT2(0.5,0.625), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },{ DirectX::XMFLOAT3(-0.75411,0.606306,-0.321015), DirectX::XMFLOAT2(0.5625,0.625), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },{ DirectX::XMFLOAT3(-0.642468,0.587462,0.293645), DirectX::XMFLOAT2(0.5625,0.46875), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-0.559352,0.82311,0.285773), DirectX::XMFLOAT2(0.6875,0.375), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },{ DirectX::XMFLOAT3(-0.642468,0.587462,0.293645), DirectX::XMFLOAT2(0.6875,0.3125), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },{ DirectX::XMFLOAT3(-0.269963,0.719175,0.230024), DirectX::XMFLOAT2(0.609375,0.375), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(-0.642468,0.587462,0.293645), DirectX::XMFLOAT2(0.6875,0.3125), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },{ DirectX::XMFLOAT3(-0.353079,0.483528,0.237896), DirectX::XMFLOAT2(0.609375,0.3125), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },{ DirectX::XMFLOAT3(-0.269963,0.719175,0.230024), DirectX::XMFLOAT2(0.609375,0.375), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(-0.75411,0.606306,-0.321015), DirectX::XMFLOAT2(0.421875,0.625), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },{ DirectX::XMFLOAT3(-0.670993,0.841954,-0.328887), DirectX::XMFLOAT2(0.421875,0.6875), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },{ DirectX::XMFLOAT3(-0.464721,0.502372,-0.376763), DirectX::XMFLOAT2(0.34375,0.625), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-0.670993,0.841954,-0.328887), DirectX::XMFLOAT2(0.421875,0.6875), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },{ DirectX::XMFLOAT3(-0.381604,0.738019,-0.384635), DirectX::XMFLOAT2(0.34375,0.6875), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },{ DirectX::XMFLOAT3(-0.464721,0.502372,-0.376763), DirectX::XMFLOAT2(0.34375,0.625), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-0.642468,0.587462,0.293645), DirectX::XMFLOAT2(0.234375,0.375), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },{ DirectX::XMFLOAT3(-0.75411,0.606306,-0.321015), DirectX::XMFLOAT2(0.234375,0.53125), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },{ DirectX::XMFLOAT3(-0.353079,0.483528,0.237896), DirectX::XMFLOAT2(0.3125,0.375), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-0.75411,0.606306,-0.321015), DirectX::XMFLOAT2(0.234375,0.53125), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },{ DirectX::XMFLOAT3(-0.464721,0.502372,-0.376763), DirectX::XMFLOAT2(0.3125,0.53125), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },{ DirectX::XMFLOAT3(-0.353079,0.483528,0.237896), DirectX::XMFLOAT2(0.3125,0.375), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-0.269963,0.719175,0.230024), DirectX::XMFLOAT2(0.15625,0.375), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },{ DirectX::XMFLOAT3(-0.381604,0.738019,-0.384635), DirectX::XMFLOAT2(0.15625,0.53125), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },{ DirectX::XMFLOAT3(-0.559352,0.82311,0.285773), DirectX::XMFLOAT2(0.234375,0.375), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.381604,0.738019,-0.384635), DirectX::XMFLOAT2(0.15625,0.53125), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },{ DirectX::XMFLOAT3(-0.670993,0.841954,-0.328887), DirectX::XMFLOAT2(0.234375,0.53125), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },{ DirectX::XMFLOAT3(-0.559352,0.82311,0.285773), DirectX::XMFLOAT2(0.234375,0.375), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.5869,0.165006,0.143497), DirectX::XMFLOAT2(0.28125,0), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },{ DirectX::XMFLOAT3(-0.653884,0.176312,-0.225299), DirectX::XMFLOAT2(0.28125,0.09375), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },{ DirectX::XMFLOAT3(-0.254433,1.107595,0.112008), DirectX::XMFLOAT2(0.53125,0), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(-0.653884,0.176312,-0.225299), DirectX::XMFLOAT2(0.28125,0.09375), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },{ DirectX::XMFLOAT3(-0.321417,1.118901,-0.256787), DirectX::XMFLOAT2(0.53125,0.09375), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },{ DirectX::XMFLOAT3(-0.254433,1.107595,0.112008), DirectX::XMFLOAT2(0.53125,0), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(-0.370188,1.149169,0.134308), DirectX::XMFLOAT2(0.28125,0.09375), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },{ DirectX::XMFLOAT3(-0.437173,1.160475,-0.234488), DirectX::XMFLOAT2(0.28125,0.1875), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },{ DirectX::XMFLOAT3(-0.702655,0.20658,0.165796), DirectX::XMFLOAT2(0.53125,0.09375), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-0.437173,1.160475,-0.234488), DirectX::XMFLOAT2(0.28125,0.1875), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },{ DirectX::XMFLOAT3(-0.76964,0.217886,-0.202999), DirectX::XMFLOAT2(0.53125,0.1875), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },{ DirectX::XMFLOAT3(-0.702655,0.20658,0.165796), DirectX::XMFLOAT2(0.53125,0.09375), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-0.370188,1.149169,0.134308), DirectX::XMFLOAT2(0.5625,0.34375), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },{ DirectX::XMFLOAT3(-0.702655,0.20658,0.165796), DirectX::XMFLOAT2(0.5625,0.09375), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },{ DirectX::XMFLOAT3(-0.254433,1.107595,0.112008), DirectX::XMFLOAT2(0.53125,0.34375), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(-0.702655,0.20658,0.165796), DirectX::XMFLOAT2(0.5625,0.09375), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },{ DirectX::XMFLOAT3(-0.5869,0.165006,0.143497), DirectX::XMFLOAT2(0.53125,0.09375), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },{ DirectX::XMFLOAT3(-0.254433,1.107595,0.112008), DirectX::XMFLOAT2(0.53125,0.34375), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(-0.76964,0.217886,-0.202999), DirectX::XMFLOAT2(0.3125,0.53125), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },{ DirectX::XMFLOAT3(-0.437173,1.160475,-0.234488), DirectX::XMFLOAT2(0.3125,0.78125), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },{ DirectX::XMFLOAT3(-0.653884,0.176312,-0.225299), DirectX::XMFLOAT2(0.28125,0.53125), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-0.437173,1.160475,-0.234488), DirectX::XMFLOAT2(0.3125,0.78125), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-0.321417,1.118901,-0.256787), DirectX::XMFLOAT2(0.28125,0.78125), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-0.653884,0.176312,-0.225299), DirectX::XMFLOAT2(0.28125,0.53125), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-0.702655,0.20658,0.165796), DirectX::XMFLOAT2(0.25,0.71875), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-0.76964,0.217886,-0.202999), DirectX::XMFLOAT2(0.25,0.8125), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-0.5869,0.165006,0.143497), DirectX::XMFLOAT2(0.28125,0.71875), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-0.76964,0.217886,-0.202999), DirectX::XMFLOAT2(0.25,0.8125), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-0.653884,0.176312,-0.225299), DirectX::XMFLOAT2(0.28125,0.8125), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-0.5869,0.165006,0.143497), DirectX::XMFLOAT2(0.28125,0.71875), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-0.254433,1.107595,0.112008), DirectX::XMFLOAT2(0.21875,0.71875), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.321417,1.118901,-0.256787), DirectX::XMFLOAT2(0.21875,0.8125), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.370188,1.149169,0.134308), DirectX::XMFLOAT2(0.25,0.71875), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.321417,1.118901,-0.256787), DirectX::XMFLOAT2(0.21875,0.8125), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.437173,1.160475,-0.234488), DirectX::XMFLOAT2(0.25,0.8125), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.370188,1.149169,0.134308), DirectX::XMFLOAT2(0.25,0.71875), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(0.021577,0.411093,0.040523), DirectX::XMFLOAT2(0.453125,0.71875), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(-0.045408,0.4224,-0.328273), DirectX::XMFLOAT2(0.453125,0.8125), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(0.063135,0.528917,0.036587), DirectX::XMFLOAT2(0.484375,0.71875), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(-0.045408,0.4224,-0.328273), DirectX::XMFLOAT2(0.453125,0.8125), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(-0.00385,0.540223,-0.332209), DirectX::XMFLOAT2(0.484375,0.8125), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(0.063135,0.528917,0.036587), DirectX::XMFLOAT2(0.484375,0.71875), DirectX::XMFLOAT3(0.9260,-0.3326,-0.1784) },
		{ DirectX::XMFLOAT3(-0.978665,0.903082,0.237282), DirectX::XMFLOAT2(0.71875,0.46875), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-1.04565,0.914388,-0.131514), DirectX::XMFLOAT2(0.71875,0.5625), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-1.020223,0.785258,0.241218), DirectX::XMFLOAT2(0.75,0.46875), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-1.04565,0.914388,-0.131514), DirectX::XMFLOAT2(0.71875,0.5625), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-1.087208,0.796564,-0.127578), DirectX::XMFLOAT2(0.75,0.5625), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-1.020223,0.785258,0.241218), DirectX::XMFLOAT2(0.75,0.46875), DirectX::XMFLOAT3(-0.9260,0.3326,0.1784) },
		{ DirectX::XMFLOAT3(-0.978665,0.903082,0.237282), DirectX::XMFLOAT2(0.28125,0.5625), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(-1.020223,0.785258,0.241218), DirectX::XMFLOAT2(0.28125,0.53125), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(0.063135,0.528917,0.036587), DirectX::XMFLOAT2(0,0.5625), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(-1.020223,0.785258,0.241218), DirectX::XMFLOAT2(0.28125,0.53125), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(0.021577,0.411093,0.040523), DirectX::XMFLOAT2(0,0.53125), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(0.063135,0.528917,0.036587), DirectX::XMFLOAT2(0,0.5625), DirectX::XMFLOAT3(0.1786,-0.0302,0.9835) },
		{ DirectX::XMFLOAT3(-1.087208,0.796564,-0.127578), DirectX::XMFLOAT2(0.8125,0), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-1.04565,0.914388,-0.131514), DirectX::XMFLOAT2(0.8125,0.03125), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-0.045408,0.4224,-0.328273), DirectX::XMFLOAT2(0.53125,0), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-1.04565,0.914388,-0.131514), DirectX::XMFLOAT2(0.8125,0.03125), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-0.00385,0.540223,-0.332209), DirectX::XMFLOAT2(0.53125,0.03125), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-0.045408,0.4224,-0.328273), DirectX::XMFLOAT2(0.53125,0), DirectX::XMFLOAT3(-0.1786,0.0302,-0.9835) },
		{ DirectX::XMFLOAT3(-1.020223,0.785258,0.241218), DirectX::XMFLOAT2(0,0.09375), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-1.087208,0.796564,-0.127578), DirectX::XMFLOAT2(0,0.1875), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(0.021577,0.411093,0.040523), DirectX::XMFLOAT2(0.28125,0.09375), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-1.087208,0.796564,-0.127578), DirectX::XMFLOAT2(0,0.1875), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(-0.045408,0.4224,-0.328273), DirectX::XMFLOAT2(0.28125,0.1875), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(0.021577,0.411093,0.040523), DirectX::XMFLOAT2(0.28125,0.09375), DirectX::XMFLOAT3(-0.3325,-0.9426,0.0315) },
		{ DirectX::XMFLOAT3(0.063135,0.528917,0.036587), DirectX::XMFLOAT2(0,0), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.00385,0.540223,-0.332209), DirectX::XMFLOAT2(0,0.09375), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.978665,0.903082,0.237282), DirectX::XMFLOAT2(0.28125,0), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.00385,0.540223,-0.332209), DirectX::XMFLOAT2(0,0.09375), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-1.04565,0.914388,-0.131514), DirectX::XMFLOAT2(0.28125,0.09375), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.978665,0.903082,0.237282), DirectX::XMFLOAT2(0.28125,0), DirectX::XMFLOAT3(0.3325,0.9426,-0.0315) },
		{ DirectX::XMFLOAT3(-0.538023,0.362813,-0.423358), DirectX::XMFLOAT2(0.3125,0.375), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.365267,0.220818,-0.724386), DirectX::XMFLOAT2(0.3125,0.46875), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.327296,0.807456,-0.512163), DirectX::XMFLOAT2(0.4375,0.375), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.365267,0.220818,-0.724386), DirectX::XMFLOAT2(0.3125,0.46875), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.15454,0.665462,-0.813191), DirectX::XMFLOAT2(0.4375,0.46875), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.327296,0.807456,-0.512163), DirectX::XMFLOAT2(0.4375,0.375), DirectX::XMFLOAT3(0.7811,-0.2565,0.5693) },
		{ DirectX::XMFLOAT3(-0.424936,0.839518,-0.58332), DirectX::XMFLOAT2(0.4375,0.375), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.25218,0.697524,-0.884349), DirectX::XMFLOAT2(0.4375,0.46875), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.635662,0.394875,-0.494516), DirectX::XMFLOAT2(0.5625,0.375), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.25218,0.697524,-0.884349), DirectX::XMFLOAT2(0.4375,0.46875), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.462907,0.25288,-0.795544), DirectX::XMFLOAT2(0.5625,0.46875), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.635662,0.394875,-0.494516), DirectX::XMFLOAT2(0.5625,0.375), DirectX::XMFLOAT3(-0.7811,0.2565,-0.5693) },
		{ DirectX::XMFLOAT3(-0.424936,0.839518,-0.58332), DirectX::XMFLOAT2(0.71875,0.40625), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.635662,0.394875,-0.494516), DirectX::XMFLOAT2(0.71875,0.28125), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.327296,0.807456,-0.512163), DirectX::XMFLOAT2(0.6875,0.40625), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.635662,0.394875,-0.494516), DirectX::XMFLOAT2(0.71875,0.28125), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.538023,0.362813,-0.423358), DirectX::XMFLOAT2(0.6875,0.28125), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.327296,0.807456,-0.512163), DirectX::XMFLOAT2(0.6875,0.40625), DirectX::XMFLOAT3(-0.4607,0.3787,0.8027) },
		{ DirectX::XMFLOAT3(-0.462907,0.25288,-0.795544), DirectX::XMFLOAT2(0.375,0.6875), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },
		{ DirectX::XMFLOAT3(-0.25218,0.697524,-0.884349), DirectX::XMFLOAT2(0.375,0.8125), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },
		{ DirectX::XMFLOAT3(-0.365267,0.220818,-0.724386), DirectX::XMFLOAT2(0.34375,0.6875), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },
		{ DirectX::XMFLOAT3(-0.25218,0.697524,-0.884349), DirectX::XMFLOAT2(0.375,0.8125), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },
		{ DirectX::XMFLOAT3(-0.15454,0.665462,-0.813191), DirectX::XMFLOAT2(0.34375,0.8125), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },
		{ DirectX::XMFLOAT3(-0.365267,0.220818,-0.724386), DirectX::XMFLOAT2(0.34375,0.6875), DirectX::XMFLOAT3(0.4607,-0.3787,-0.8027) },
		{ DirectX::XMFLOAT3(-0.635662,0.394875,-0.494516), DirectX::XMFLOAT2(0.1875,0.71875), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },
		{ DirectX::XMFLOAT3(-0.462907,0.25288,-0.795544), DirectX::XMFLOAT2(0.1875,0.8125), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },
		{ DirectX::XMFLOAT3(-0.538023,0.362813,-0.423358), DirectX::XMFLOAT2(0.21875,0.71875), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },
		{ DirectX::XMFLOAT3(-0.462907,0.25288,-0.795544), DirectX::XMFLOAT2(0.1875,0.8125), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },
		{ DirectX::XMFLOAT3(-0.365267,0.220818,-0.724386), DirectX::XMFLOAT2(0.21875,0.8125), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },
		{ DirectX::XMFLOAT3(-0.538023,0.362813,-0.423358), DirectX::XMFLOAT2(0.21875,0.71875), DirectX::XMFLOAT3(-0.4215,-0.8893,0.1776) },
		{ DirectX::XMFLOAT3(-0.327296,0.807456,-0.512163), DirectX::XMFLOAT2(0.15625,0.71875), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },
		{ DirectX::XMFLOAT3(-0.15454,0.665462,-0.813191), DirectX::XMFLOAT2(0.15625,0.8125), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },
		{ DirectX::XMFLOAT3(-0.424936,0.839518,-0.58332), DirectX::XMFLOAT2(0.1875,0.71875), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },
		{ DirectX::XMFLOAT3(-0.15454,0.665462,-0.813191), DirectX::XMFLOAT2(0.15625,0.8125), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },
		{ DirectX::XMFLOAT3(-0.25218,0.697524,-0.884349), DirectX::XMFLOAT2(0.1875,0.8125), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },
		{ DirectX::XMFLOAT3(-0.424936,0.839518,-0.58332), DirectX::XMFLOAT2(0.1875,0.71875), DirectX::XMFLOAT3(0.4215,0.8893,-0.1776) },
		*/
	};

	// 버퍼를 설정하는 구조체
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(cube);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// 하위 리소스 설정. 텍스처 같은거 할때나 쓰임
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = cube;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// 위에 설정한 버퍼 설정을 바탕으로 버퍼를 초기화함.
	hr = m_pd3dDevice->CreateBuffer(
		&bufferDesc, 
		&InitData, 
		m_cubeVertexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	UINT indices[] =
	{
		/*
			0, 1, 3,		// 밑
			3, 1, 2,

			4, 5, 0,		// 전
			0, 5, 1,

			7, 4, 3,		// 좌
			3, 4, 0,

			6, 7, 2,		// 후
			2, 7, 3,

			5, 6, 1,		// 우
			1, 6, 2,

			7, 6, 5,		// 상
			7, 5, 4,
		*/
			0, 2, 1, 
			2, 3, 1, 
			/*
			4, 6, 5, 
			6, 7, 5, 
			8, 10, 9, 
			10, 11, 9, 
			12, 14, 13, 
			14, 15, 13, 
			16, 18, 17, 
			18, 19, 17, 
			20, 22, 21, 
			22, 23, 21, 
			24, 26, 25, 
			26, 27, 25, 
			28, 30, 29, 
			30, 31, 29, 
			32, 34, 33, 
			34, 35, 33, 
			36, 38, 37, 
			38, 39, 37, 
			40, 42, 41, 
			42, 43, 41, 
			44, 46, 45, 
			46, 47, 45, 
			48, 50, 49, 
			50, 51, 49, 
			52, 54, 53, 
			54, 55, 53, 
			56, 58, 57, 
			58, 59, 57, 
			60, 62, 61, 
			62, 63, 61, 
			64, 66, 65, 
			66, 67, 65, 
			68, 70, 69, 
			70, 71, 69, 
			72, 74, 73, 
			74, 75, 73, 
			76, 78, 77, 
			78, 79, 77, 
			80, 82, 81, 
			82, 83, 81, 
			84, 86, 85, 
			86, 87, 85, 
			88, 90, 89, 
			90, 91, 89, 
			92, 94, 93, 
			94, 95, 93, 
			96, 98, 97, 
			98, 99, 97, 
			100, 102, 101, 
			102, 103, 101, 
			104, 106, 105, 
			106, 107, 105, 
			108, 110, 109, 
			110, 111, 109, 
			112, 114, 113, 
			114, 115, 113, 
			116, 118, 117, 
			118, 119, 117, 
			120, 122, 121, 
			122, 123, 121, 
			124, 126, 125, 
			126, 127, 125, 
			128, 130, 129, 
			130, 131, 129, 
			132, 134, 133, 
			134, 135, 133, 
			136, 138, 137, 
			138, 139, 137, 
			140, 142, 141, 
			142, 143, 141, 
			144, 146, 145, 
			146, 147, 145, 
			148, 150, 149, 
			150, 151, 149, 
			152, 154, 153, 
			154, 155, 153, 
			156, 158, 157, 
			158, 159, 157, 
			160, 162, 161, 
			162, 163, 161, 
			164, 166, 165, 
			166, 167, 165, 
			168, 170, 169, 
			170, 171, 169, 
			172, 174, 173, 
			174, 175, 173, 
			176, 178, 177, 
			178, 179, 177, 
			180, 182, 181, 
			182, 183, 181, 
			184, 186, 185, 
			186, 187, 185, 
			188, 190, 189, 
			190, 191, 189, 
			192, 194, 193, 
			194, 195, 193, 
			196, 198, 197, 
			198, 199, 197, 
			200, 202, 201, 
			202, 203, 201, 
			204, 206, 205, 
			206, 207, 205, 
			208, 210, 209, 
			210, 211, 209, 
			212, 214, 213, 
			214, 215, 213, 
			216, 218, 217, 
			218, 219, 217, 
			220, 222, 221, 
			222, 223, 221, 
			224, 226, 225, 
			226, 227, 225, 
			228, 230, 229, 
			230, 231, 229, 
			232, 234, 233, 
			234, 235, 233, 
			236, 238, 237, 
			238, 239, 237, 
			240, 242, 241, 
			242, 243, 241, 
			244, 246, 245, 
			246, 247, 245, 
			248, 250, 249, 
			250, 251, 249, 
			252, 254, 253, 
			254, 255, 253, 
			256, 258, 257, 
			258, 259, 257, 
			260, 262, 261, 
			262, 263, 261, 
			264, 266, 265, 
			266, 267, 265, 
			268, 270, 269, 
			270, 271, 269, 
			272, 274, 273, 
			274, 275, 273, 
			276, 278, 277, 
			278, 279, 277, 
			280, 282, 281, 
			282, 283, 281, 
			284, 286, 285, 
			286, 287, 285, 
			288, 290, 289, 
			290, 291, 289, 
			292, 294, 293, 
			294, 295, 293, 
			296, 298, 297, 
			298, 299, 297, 
			300, 302, 301, 
			302, 303, 301, 
			304, 306, 305, 
			306, 307, 305, 
			308, 310, 309, 
			310, 311, 309, 
			312, 314, 313, 
			314, 315, 313, 
			316, 318, 317, 
			318, 319, 317, 
			320, 322, 321, 
			322, 323, 321, 
			324, 326, 325, 
			326, 327, 325, 
			328, 330, 329, 
			330, 331, 329, 
			332, 334, 333, 
			334, 335, 333, 
			336, 338, 337, 
			338, 339, 337, 
			340, 342, 341, 
			342, 343, 341, 
			344, 346, 345, 
			346, 347, 345, 
			348, 350, 349, 
			350, 351, 349, 
			352, 354, 353, 
			354, 355, 353, 
			356, 358, 357, 
			358, 359, 357, 
			360, 362, 361, 
			362, 363, 361, 
			364, 366, 365, 
			366, 367, 365, 
			368, 370, 369, 
			370, 371, 369, 
			372, 374, 373, 
			374, 375, 373, 
			376, 378, 377, 
			378, 379, 377, 
			380, 382, 381, 
			382, 383, 381, 
			384, 386, 385, 
			386, 387, 385, 
			388, 390, 389, 
			390, 391, 389, 
			392, 394, 393, 
			394, 395, 393, 
			396, 398, 397, 
			398, 399, 397, 
			400, 402, 401, 
			402, 403, 401, 
			404, 406, 405, 
			406, 407, 405, 
			408, 410, 409, 
			410, 411, 409, 
			412, 414, 413, 
			414, 415, 413, 
			416, 418, 417, 
			418, 419, 417, 
			420, 422, 421, 
			422, 423, 421, 
			424, 426, 425, 
			426, 427, 425, 
			428, 430, 429, 
			430, 431, 429, 
			432, 434, 433, 
			434, 435, 433, 
			436, 438, 437, 
			438, 439, 437, 
			440, 442, 441, 
			442, 443, 441, 
			444, 446, 445, 
			446, 447, 445, 
			448, 450, 449, 
			450, 451, 449, 
			452, 454, 453, 
			454, 455, 453, 
			456, 458, 457, 
			458, 459, 457, 
			460, 462, 461, 
			462, 463, 461, 
			464, 466, 465, 
			466, 467, 465, 
			468, 470, 469, 
			470, 471, 469, 
			472, 474, 473, 
			474, 475, 473, 
			476, 478, 477, 
			478, 479, 477, 
			480, 482, 481, 
			482, 483, 481, 
			484, 486, 485, 
			486, 487, 485, 
			488, 490, 489, 
			490, 491, 489, 
			492, 494, 493, 
			494, 495, 493, 
			496, 498, 497, 
			498, 499, 497, 
			500, 502, 501, 
			502, 503, 501, 
			504, 506, 505, 
			506, 507, 505, 
			508, 510, 509, 
			510, 511, 509, 
			512, 514, 513, 
			514, 515, 513, 
			516, 518, 517, 
			518, 519, 517, 
			520, 522, 521, 
			522, 523, 521, 
			524, 526, 525, 
			526, 527, 525, 
			528, 530, 529, 
			530, 531, 529, 
			532, 534, 533, 
			534, 535, 533, 
			536, 538, 537, 
			538, 539, 537, 
			540, 542, 541, 
			542, 543, 541, 
			544, 546, 545, 
			546, 547, 545, 
			548, 550, 549, 
			550, 551, 549, 
			552, 554, 553, 
			554, 555, 553, 
			556, 558, 557, 
			558, 559, 557, 
			560, 562, 561, 
			562, 563, 561, 
			564, 566, 565, 
			566, 567, 565, 
			568, 570, 569, 
			570, 571, 569, 
			572, 574, 573, 
			574, 575, 573, 
			576, 578, 577, 
			578, 579, 577, 
			580, 582, 581, 
			582, 583, 581, 
			584, 586, 585, 
			586, 587, 585, 
			588, 590, 589, 
			590, 591, 589, 
			592, 594, 593, 
			594, 595, 593, 
			596, 598, 597, 
			598, 599, 597, 
			600, 602, 601, 
			602, 603, 601, 
			604, 606, 605, 
			606, 607, 605, 
			608, 610, 609, 
			610, 611, 609, 
			612, 614, 613, 
			614, 615, 613, 
			616, 618, 617, 
			618, 619, 617, 
			620, 622, 621, 
			622, 623, 621, 
			624, 626, 625, 
			626, 627, 625, 
			628, 630, 629, 
			630, 631, 629, 
			632, 634, 633, 
			634, 635, 633, 
			636, 638, 637, 
			638, 639, 637, 
			640, 642, 641, 
			642, 643, 641, 
			644, 646, 645, 
			646, 647, 645, 
			648, 650, 649, 
			650, 651, 649, 
			652, 654, 653, 
			654, 655, 653, 
			656, 658, 657, 
			658, 659, 657, 
			660, 662, 661, 
			662, 663, 661, 
			664, 666, 665, 
			666, 667, 665, 
			668, 670, 669, 
			670, 671, 669, 
			672, 674, 673, 
			674, 675, 673, 
			676, 678, 677, 
			678, 679, 677, 
			680, 682, 681, 
			682, 683, 681, 
			684, 686, 685, 
			686, 687, 685, 
			688, 690, 689, 
			690, 691, 689, 
			692, 694, 693, 
			694, 695, 693, 
			696, 698, 697, 
			698, 699, 697, 
			700, 702, 701, 
			702, 703, 701, 
			704, 706, 705, 
			706, 707, 705, 
			708, 710, 709, 
			710, 711, 709, 
			712, 714, 713, 
			714, 715, 713, 
			716, 718, 717, 
			718, 719, 717, 
			720, 722, 721, 
			722, 723, 721, 
			724, 726, 725, 
			726, 727, 725, 
			728, 730, 729, 
			730, 731, 729, 
			732, 734, 733, 
			734, 735, 733, 
			736, 738, 737, 
			738, 739, 737, 
			740, 742, 741, 
			742, 743, 741, 
			744, 746, 745, 
			746, 747, 745, 
			748, 750, 749, 
			750, 751, 749, 
			752, 754, 753, 
			754, 755, 753, 
			756, 758, 757, 
			758, 759, 757, 
			760, 762, 761, 
			762, 763, 761, 
			764, 766, 765, 
			766, 767, 765, 
			768, 770, 769, 
			770, 771, 769, 
			772, 774, 773, 
			774, 775, 773, 
			776, 778, 777, 
			778, 779, 777, 
			780, 782, 781, 
			782, 783, 781, 
			784, 786, 785, 
			786, 787, 785, 
			788, 790, 789, 
			790, 791, 789, 
			792, 794, 793, 
			794, 795, 793, 
			796, 798, 797, 
			798, 799, 797, 
			800, 802, 801, 
			802, 803, 801, 
			804, 806, 805, 
			806, 807, 805, 
			808, 810, 809, 
			810, 811, 809, 
			812, 814, 813, 
			814, 815, 813, 
			816, 818, 817, 
			818, 819, 817, 
			820, 822, 821, 
			822, 823, 821, 
			824, 826, 825, 
			826, 827, 825, 
			828, 830, 829, 
			830, 831, 829, 
			832, 834, 833, 
			834, 835, 833, 
			836, 838, 837, 
			838, 839, 837, 
			840, 842, 841, 
			842, 843, 841, 
			844, 846, 845, 
			846, 847, 845, 
			848, 850, 849, 
			850, 851, 849, 
			852, 854, 853, 
			854, 855, 853, 
			856, 858, 857, 
			858, 859, 857, 
			860, 862, 861, 
			862, 863, 861, 
			*/
	};

	cubeIndexCount = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInit;
	indexInit.pSysMem = indices;
	indexInit.SysMemPitch = 0;
	indexInit.SysMemSlicePitch = 0;

	hr = m_pd3dDevice->CreateBuffer(
		&indexBufferDesc, 
		&indexInit, 
		m_cubeIndexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	ComPtr<ID3D11Resource> texture;
	ComPtr<ID3D11ShaderResourceView> textureView;				// ../ 같은 경로는 다시 생각해보자.. 실행파일 뽑을 때 많이 귀찮아 진다...
	hr = DirectX::CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"../Texture/WoodCrate01.dds", texture.GetAddressOf(), textureView.GetAddressOf());
	// hr = DirectX::CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"WoodCrate01.dds", texture.GetAddressOf(), textureView.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	texture.As(&m_cubeTexture);
	textureView.As(&m_cubeTextureView);

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.f;
	samplerDesc.BorderColor[1] = 1.f;
	samplerDesc.BorderColor[2] = 1.f;
	samplerDesc.BorderColor[3] = 1.f;

	ComPtr<ID3D11SamplerState> samplerstate;
	hr = m_pd3dDevice->CreateSamplerState(&samplerDesc, samplerstate.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	samplerstate.As(&m_cubeSamplerState);

	return hr;
}

HRESULT DXGraphics::CreateAxis()
{
	HRESULT hr = S_OK;
	
	// 꼭짓점을 설명하는 정보
	Vertex axis[] =
	{
		{DirectX::XMFLOAT3(0.f, 0.f, 0.f),	DirectX::XMFLOAT4(Color::Red),},
		{DirectX::XMFLOAT3(15.f, 0.f, 0.f), DirectX::XMFLOAT4(Color::Red),},

		{DirectX::XMFLOAT3(0.f, 0.f, 0.f),	DirectX::XMFLOAT4(Color::Green),},
		{DirectX::XMFLOAT3(0.f ,15.f, 0.f), DirectX::XMFLOAT4(Color::Green),},

		{DirectX::XMFLOAT3(0.f, 0.f, 0.f),	DirectX::XMFLOAT4(Color::Blue),},
		{DirectX::XMFLOAT3(0.f ,0.f, 15.f), DirectX::XMFLOAT4(Color::Blue),},
	};

	// 버퍼를 설정하는 구조체
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(axis);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// 하위 리소스 설정. 텍스처 같은거 할때나 쓰임
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = axis;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// 위에 설정한 버퍼 설정을 바탕으로 버퍼를 초기화함.
	hr = m_pd3dDevice->CreateBuffer(
		&bufferDesc,
		&InitData,
		m_axisVertexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	UINT indices[] = {
		0, 1,			// x

		2, 3,			// y

		4, 5,			// z
	};

	axisIndexCount = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInit;
	indexInit.pSysMem = indices;
	indexInit.SysMemPitch = 0;
	indexInit.SysMemSlicePitch = 0;

	hr = m_pd3dDevice->CreateBuffer(
		&indexBufferDesc,
		&indexInit,
		m_axisIndexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT DXGraphics::CreateGrid()
{
	HRESULT hr = S_OK;
	
	// 꼭짓점을 설명하는 정보
	Vertex vertices[100];
	for (int i = 0; i < 100; i++)
	{
		vertices[i].pos = DirectX::XMFLOAT3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
		vertices[i].color = DirectX::XMFLOAT4(Color::White);
	}

	// 버퍼를 설정하는 구조체
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// 하위 리소스 설정. 텍스처 같은거 할때나 쓰임
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// 위에 설정한 버퍼 설정을 바탕으로 버퍼를 초기화함.
	hr = m_pd3dDevice->CreateBuffer(
		&bufferDesc,
		&InitData,
		m_gridVertexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	UINT indices[40];
	for (int i = 0; i < 10; i++)
	{
		indices[i * 2] = i;
		indices[i * 2 + 1] = i + 90;
	}

	for (int i = 0; i < 10; i++)
	{
		indices[20 + (i * 2)] = i * 10;
		indices[20 + (i * 2) + 1] = i * 10 + 9;
	}

	gridIndexCount = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInit;
	indexInit.pSysMem = indices;
	indexInit.SysMemPitch = 0;
	indexInit.SysMemSlicePitch = 0;

	hr = m_pd3dDevice->CreateBuffer(
		&indexBufferDesc,
		&indexInit,
		m_gridIndexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
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
		Color::SaddleBrown
	);

	// 뎁스 스텐실 뷰 클리어
	m_pd3dDeviceContext->ClearDepthStencilView(
		m_pd3dDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
		1.0f, 
		0
	);


	/// 쉐이더 코드 자리?? 인듯??	
	{
		// 레스터라이즈 스테이트
		m_pd3dDeviceContext->RSSetState(m_wireRasterizerState.Get());

		m_pd3dDeviceContext->IASetInputLayout(m_axisInputLayout.Get());
		m_pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		// IA에 버텍스 버퍼 설정
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_pd3dDeviceContext->IASetVertexBuffers(
			0,
			1,
			m_axisVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		// IA에 인덱스 버퍼 설정
		m_pd3dDeviceContext->IASetIndexBuffer(
			m_axisIndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT,					// 32비트 unsigned int 형으로 읽음
			0
		);

		// 상수 버퍼 설정
		DirectX::XMMATRIX worldViewProj = m_constantBufferData.world * m_constantBufferData.view * m_constantBufferData.projection;
		m_axisMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		// 테크닉
		D3DX11_TECHNIQUE_DESC techDesc;
		m_axisTechnique->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_axisTechnique->GetPassByIndex(p)->Apply(0, m_pd3dDeviceContext.Get());

			m_pd3dDeviceContext->DrawIndexed(axisIndexCount, 0, 0);
		}
	}	

	{
		// 레스터라이즈 스테이트
		m_pd3dDeviceContext->RSSetState(m_wireRasterizerState.Get());

		m_pd3dDeviceContext->IASetInputLayout(m_gridInputLayout.Get());
		m_pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		// IA에 버텍스 버퍼 설정
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_pd3dDeviceContext->IASetVertexBuffers(
			0,
			1,
			m_gridVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		// IA에 인덱스 버퍼 설정
		m_pd3dDeviceContext->IASetIndexBuffer(
			m_gridIndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT,					// 32비트 unsigned int 형으로 읽음
			0
		);

		// 상수 버퍼 설정
		DirectX::XMMATRIX worldViewProj = m_constantBufferData.world * m_constantBufferData.view * m_constantBufferData.projection;
		m_gridMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		// 테크닉
		D3DX11_TECHNIQUE_DESC techDesc;
		m_gridTechnique->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_gridTechnique->GetPassByIndex(p)->Apply(0, m_pd3dDeviceContext.Get());

			m_pd3dDeviceContext->DrawIndexed(gridIndexCount, 0, 0);
		}
	}

	{
		// 레스터라이즈 스테이트
		m_pd3dDeviceContext->RSSetState(m_currRasterizerState.Get());

		m_pd3dDeviceContext->IASetInputLayout(m_cubeInputLayout.Get());
		m_pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// IA에 버텍스 버퍼 설정
		UINT stride = sizeof(TextureVertex);
		UINT offset = 0;
		m_pd3dDeviceContext->IASetVertexBuffers(
			0,
			1,
			m_cubeVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		// IA에 인덱스 버퍼 설정
		m_pd3dDeviceContext->IASetIndexBuffer(
			m_cubeIndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT,					// 32비트 unsigned int 형으로 읽음
			0
		);

		// 상수 버퍼 설정
		DirectX::XMMATRIX worldViewProj = m_constantBufferData.world * m_constantBufferData.view * m_constantBufferData.projection;
		m_cubeMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		// m_cubeSampler->SetSampler(0, m_cubeSamplerState.Get());
		m_cubeShaderResource->SetResource(m_cubeTextureView.Get());

		// 테크닉
		D3DX11_TECHNIQUE_DESC techDesc;
		m_cubeTechnique->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_cubeTechnique->GetPassByIndex(p)->Apply(0, m_pd3dDeviceContext.Get());
			
			m_pd3dDeviceContext->DrawIndexed(cubeIndexCount, 0, 0);
		}
	}

	/*
	m_pd3dDeviceContext->VSSetShader(
		m_vertexShader.Get(), 
		nullptr, 
		0
	);

	m_pd3dDeviceContext->VSSetConstantBuffers(
		0, 
		1, 
		m_constantBuffer.GetAddressOf()
	);

	m_pd3dDeviceContext->PSSetShader(
		m_pixelShader.Get(), 
		nullptr,
		0
	);

	m_pd3dDeviceContext->DrawIndexed(
		count, 
		0, 
		0
	);
	*/
	
	/// 스왑체인1의 프리센트1을 사용하기 위해서는 이것도 필요함.
	// DXGI_PRESENT_PARAMETERS present;
	// present.DirtyRectsCount = 0;
	// present.pDirtyRects = nullptr;
	// present.pScrollRect = nullptr;
	// present.pScrollOffset = nullptr;
	// m_pDXGISwapChain1->Present1(1, 0, &present);
}

void DXGraphics::EndDraw()
{
	m_pDXGISwapChain1->Present(1, 0);
}

 