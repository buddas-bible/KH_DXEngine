#include "DXGraphics.h"

#include <fstream>
#include <vector>

#include "InputManager.h"
#include "TimeController.h"

using namespace Microsoft::WRL;

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

		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		2,										// 버텍스에 들어간 데이터 갯수
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
		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f),	 DirectX::XMFLOAT4(Color::Black),},
		{DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f),  DirectX::XMFLOAT4(Color::Red),},
		{DirectX::XMFLOAT3(0.5f,-0.5f, 0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f),	 DirectX::XMFLOAT4(Color::Magenta),},
		{DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.5f, 1.f),  DirectX::XMFLOAT4(Color::Blue),},

		{DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f),  DirectX::XMFLOAT4(Color::Green),},
		{DirectX::XMFLOAT3(0.5f, 0.5f,-0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f),	 DirectX::XMFLOAT4(Color::Yellow),},
		{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.7f, 0.7f), DirectX::XMFLOAT4(Color::White),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f),	DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f),	 DirectX::XMFLOAT4(Color::Cyan),},
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

 