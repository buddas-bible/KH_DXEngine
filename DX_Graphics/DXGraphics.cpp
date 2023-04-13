#include "DXGraphics.h"

#include <fstream>
#include <vector>

#include "Axis.h"
#include "Grid.h"

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
		MessageBox(m_hWnd, L"����̽� ���� ����", L"�ʱ�ȭ ����", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateSwapChain();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"����ü�� ���� ����", L"�ʱ�ȭ ����", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateRenderTargetView();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"����Ÿ�ٺ� ���� ����", L"�ʱ�ȭ ����", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateDepthStencilView();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"�������ٽǺ� ���� ����", L"�ʱ�ȭ ����", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateRasterState();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"�����ͽ�����Ʈ ���� ����", L"�ʱ�ȭ ����", MB_OK | MB_ICONWARNING);
		return hr;
	}

	/// �̺κ� ���ʹ� �׳� �ٲ���� ����
	hr = CreateObject();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateInputLayout();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"��ǲ���̾ƿ� ���� ����", L"�ʱ�ȭ ����", MB_OK | MB_ICONWARNING);
		return hr;
	}

	directionalLight = Vector3D(1.f, 1.f, 0.f).Normalize();
	pointLight; // �̰� ���忡���� ��ġ�� ������ �־����.

	return hr;
}

void DXGraphics::Finalize()
{

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

	float dt = time.GetfDeltaTime();
	if (GetAsyncKeyState(VK_Q))
	{
		camera.Yaw(4 * dt);
	}
	if (GetAsyncKeyState(VK_E))
	{
		camera.Yaw(-4 * dt);
	}
	if (GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_W))
	{
		camera.Pitch(4 * dt);
	}
	if (GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_S))
	{
		camera.Pitch(-4 * dt);
	}

	if (!GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_A))
	{
		camera.moveR(-4 * dt);
	}
	if (!GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_D))
	{
		camera.moveR(4 * dt);
	}
	if (!GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_W))
	{
		camera.moveL(4 * dt);
	}
	if (!GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_S))
	{
		camera.moveL(-4 * dt);
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		camera.moveX(-3.f * dt);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		camera.moveX(3.f * dt);
	}
	if (GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_UP))
	{
		camera.moveY(3.f * dt);
	}
	if (GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_DOWN))
	{
		camera.moveY(-3.f * dt);
	}
	if (GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_SHIFT))
	{
		camera.moveZ(3.f * dt);
	}
	if (GetAsyncKeyState(VK_DOWN) && !GetAsyncKeyState(VK_SHIFT))
	{
		camera.moveZ(-3.f * dt);
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

	if (GetAsyncKeyState(0x31) && !GetAsyncKeyState(VK_SHIFT))
	{
		directionalLight = Vector4D{ 1.f, 1.f, 0.f, 1.f }.Normalize();
	}
	if (GetAsyncKeyState(0x32) && !GetAsyncKeyState(VK_SHIFT))
	{
		directionalLight = Vector4D{ 0.f, 1.f, 1.f, 1.f }.Normalize();
	}
	if (GetAsyncKeyState(0x33) && !GetAsyncKeyState(VK_SHIFT))
	{
		directionalLight = Vector4D{ 1.f, 0.f, 1.f, 1.f }.Normalize();
	}

	if (GetAsyncKeyState(0x34) && !GetAsyncKeyState(VK_SHIFT))
	{
		directionalLight = Vector4D{ -1.f, 1.f, 1.f, 1.f }.Normalize();
	}
	if (GetAsyncKeyState(0x35) && !GetAsyncKeyState(VK_SHIFT))
	{
		directionalLight = Vector4D{ 1.f, -1.f, 1.f, 1.f }.Normalize();
	}
	if (GetAsyncKeyState(0x36) && !GetAsyncKeyState(VK_SHIFT))
	{
		directionalLight = Vector4D{ 1.f, 1.f, -1.f, 1.f }.Normalize();
	}

	if (GetAsyncKeyState(0x37) && !GetAsyncKeyState(VK_SHIFT))
	{
		directionalLight = Vector4D{ -1.f, -1.f, 1.f, 0.f }.Normalize();
	}
	if (GetAsyncKeyState(0x38) && !GetAsyncKeyState(VK_SHIFT))
	{
		directionalLight = Vector4D{ 1.f, -1.f, -1.f, 0.f }.Normalize();
	}
	if (GetAsyncKeyState(0x39) && !GetAsyncKeyState(VK_SHIFT))
	{
		directionalLight = Vector4D{ -1.f, 1.f, -1.f, 0.f }.Normalize();
	}
}

HRESULT DXGraphics::CreateDevice()
{
	HRESULT hr = S_OK;

	// ����̽� ��� ��������
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

	// �ӽ� ������ ����
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	// ����̽�, ����̽� ���ؽ�Ʈ ����
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

	// ������� �����͸� �ɹ� ������ ������
	device.As(&m_pd3dDevice);
	context.As(&m_pd3dDeviceContext);

	return hr;
}

HRESULT DXGraphics::CreateSwapChain()
{
	HRESULT hr = S_OK;

	// ���� ü�� ����
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
	dxgiSwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // �� ��쿡 format�� B8G8R8A8, R8G8B8A8�� �ؤ���
	dxgiSwapChainDesc1.Flags = 0;

	// �ӽ� ������ ����
	ComPtr<IDXGIDevice2> device2;
	m_pd3dDevice.As(&device2);

	device2->SetMaximumFrameLatency(1);

	// ����̽��� ��͸� ������
	ComPtr<IDXGIAdapter> adapter;
	device2->GetAdapter(&adapter);

	// ��Ϳ��� ���丮�� ����
	ComPtr<IDXGIFactory2> factory2;
	adapter->GetParent(
		__uuidof(IDXGIFactory2),
		&factory2
	);

	// HWND�� ����ü�� ����
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

	// ������� �����͸� �ɹ� ������ ������
	swapChain1.As(&m_pDXGISwapChain1);

	return hr;
}

HRESULT DXGraphics::CreateRenderTargetView()
{
	HRESULT hr = S_OK;

	// ����ü���� ������ �ִ� ���۸� ������
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

	// ������� ����۷κ��� ������ �޾ƿ�
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

	// ������� ������ ������
	D3D11_TEXTURE2D_DESC backBufferDesc{};
	ZeroMemory(&backBufferDesc, sizeof(backBufferDesc));
	m_backBuffer->GetDesc(&backBufferDesc);

	// ������� ������
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
	// ���� ���
	// �̰� ������Ʈ���� ������ ������.
	m_world = Matrix4x4::IdentityMatrix();

	Vector3D eye{ 0.0f, 0.7f, -1.5f };
	Vector3D at{ 0.0f, 0.1f, 0.0f };
	Vector3D up{ 0.0f, 1.0f, 0.0f };
	camera.CameraLookAtLH(eye, at, up);

	m_view = camera.GetViewMatrix();

	// ȭ�� ����
	float aspectRatioX = static_cast<float>(m_d3dBackBufferDesc.Width) / static_cast<float>(m_d3dBackBufferDesc.Height);
	float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

	// ���� ��Ʈ����
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

	D3D11_RASTERIZER_DESC solidRasterDesc;				// ä��� ���
	ZeroMemory(&solidRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidRasterDesc.FillMode = D3D11_FILL_SOLID;			// ä��� ���
	solidRasterDesc.CullMode = D3D11_CULL_BACK;			// ������ ������ �׸��� ���� (BACK�̴� �޸��� �׸��� ����)	
	solidRasterDesc.FrontCounterClockwise = false;		// �ð�������� �� ����
	solidRasterDesc.DepthClipEnable = true;				// �Ÿ��� ���� Ŭ������ ����

	hr = m_pd3dDevice->CreateRasterizerState(
		&solidRasterDesc,
		m_solidRasterizerState.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_RASTERIZER_DESC wireRasterDesc;						// ä���� �ʴ� ���
	ZeroMemory(&wireRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireRasterDesc.FillMode = D3D11_FILL_WIREFRAME;			// ä��� ���
	wireRasterDesc.CullMode = D3D11_CULL_NONE;				// ������ ������ �׸��� ����
	wireRasterDesc.FrontCounterClockwise = false;				// �ð�������� �� ����
	wireRasterDesc.DepthClipEnable = true;					// �Ÿ��� ���� Ŭ������ ����

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
		MessageBox(m_hWnd, L"ť�� ������Ʈ �ʱ�ȭ ����", L"������Ʈ ���� ����", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateAxis();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"�� ������Ʈ �ʱ�ȭ ����", L"������Ʈ ���� ����", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateGrid();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"�׸��� ������Ʈ �ʱ�ȭ ����", L"������Ʈ ���� ����", MB_OK | MB_ICONWARNING);
		return hr;
	}

	hr = CreateCubeShaders();
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"���̴� �ʱ�ȭ ����", L"������Ʈ ���� ����", MB_OK | MB_ICONWARNING);
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

	// ��ǲ ���̾ƿ� ����
	D3DX11_PASS_DESC cubePassDesc;
	m_cubeTechnique->GetPassByIndex(0)->GetDesc(&cubePassDesc);
	hr = m_pd3dDevice->CreateInputLayout(
		textureDesc,
		ARRAYSIZE(textureDesc),						// ���ؽ��� �� ������ ����
		cubePassDesc.pIAInputSignature,				// ���̴� �ڵ� ������
		cubePassDesc.IAInputSignatureSize,			// ���̴� ũ��
		m_cubeInputLayout.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}	

	return hr;
}

/// <summary>
/// �ʱ�ȭ �� �� ���� �Լ�
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
	std::ifstream cfin("../x64/release/Texture.cso", std::ios::binary);		// �̺κ� ��� �ٽ� �����غ���...
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

	/// ���Ϸκ��� ����Ʈ ������ �о��
	// ť�� ����Ʈ
	{
		hr = D3DX11CreateEffectFromMemory(
			&cCompiledShader[0],
			size,
			0,
			m_pd3dDevice.Get(),
			m_cubeEffect.GetAddressOf()
		);

		m_cubeTechnique = m_cubeEffect->GetTechniqueByName("Tech");								// ���Ͽ� Tech �̸��� �����͸� �о��
		m_cubeMatrixVariable = m_cubeEffect->GetVariableByName("worldViewProj")->AsMatrix();	// ���Ͽ� worldViewProj �̸��� �����͸� �о��
		
		// ����Ʈ�� ���̴� ���ҽ��� ���÷��� �ʿ����� ������?
		m_cubeShaderResource = m_cubeEffect->GetVariableByName("g_Texture")->AsShaderResource();	// 
		m_cubeSampler = m_cubeEffect->GetVariableByName("g_Sampler")->AsSampler();

		m_directionalLight = m_cubeEffect->GetVariableByName("fLight")->AsVector();
	}

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT DXGraphics::CreateCube()
{
	HRESULT hr = S_OK;

	// �������� �����ϴ� ����
	PTNVertex cube[] =
	{
		// POSITION								UV								NORMAL
		{ Vector3D(-0.5f,-0.5f,-0.5f),	Vector2D(0.f, 0.f),	Vector3D(0.f, -1.f, 0.f), },	// 0	0
		{ Vector3D(0.5f,-0.5f,-0.5f),	Vector2D(1.f, 0.f),	Vector3D(0.f, -1.f, 0.f), },	// 1	1
		{ Vector3D(0.5f,-0.5f, 0.5f),	Vector2D(1.f, 1.f),	Vector3D(0.f, -1.f, 0.f), },	// 2	2
		{ Vector3D(-0.5f,-0.5f, 0.5f),	Vector2D(0.f, 1.f),	Vector3D(0.f, -1.f, 0.f), },	// 3	3
		  
		{ Vector3D(-0.5f, 0.5f,-0.5f),	Vector2D(0.f, 0.f),	Vector3D(0.f, 0.f, -1.f), },	// 4	4
		{ Vector3D(0.5f, 0.5f,-0.5f),	Vector2D(1.f, 0.f),	Vector3D(0.f, 0.f, -1.f), },	// 5	5
		{ Vector3D(0.5f,-0.5f,-0.5f),	Vector2D(1.f, 1.f),	Vector3D(0.f, 0.f, -1.f), },	// 1	6
		{ Vector3D(-0.5f,-0.5f,-0.5f),	Vector2D(0.f, 1.f),	Vector3D(0.f, 0.f, -1.f), },	// 0	7

		{ Vector3D(0.5f, 0.5f,-0.5f),	Vector2D(0.f, 0.f),	Vector3D(1.f, 0.f, 0.f), },	// 5	8
		{ Vector3D(0.5f, 0.5f, 0.5f),	Vector2D(1.f, 0.f),	Vector3D(1.f, 0.f, 0.f), },	// 6	9
		{ Vector3D(0.5f,-0.5f, 0.5f),	Vector2D(1.f, 1.f),	Vector3D(1.f, 0.f, 0.f), },	// 2	10
		{ Vector3D(0.5f,-0.5f,-0.5f),	Vector2D(0.f, 1.f),	Vector3D(1.f, 0.f, 0.f), },	// 1	11

		{ Vector3D(0.5f, 0.5f, 0.5f),	Vector2D(0.f, 0.f),	Vector3D(0.f, 0.f, 1.f), },	// 6	12
		{ Vector3D(-0.5f, 0.5f, 0.5f),	Vector2D(0.f, 1.f),	Vector3D(0.f, 0.f, 1.f), },	// 7	13
		{ Vector3D(-0.5f,-0.5f, 0.5f),	Vector2D(1.f, 1.f),	Vector3D(0.f, 0.f, 1.f), },	// 3	14
		{ Vector3D(0.5f,-0.5f, 0.5f),	Vector2D(0.f, 1.f),	Vector3D(0.f, 0.f, 1.f), },	// 2	15
		
		{ Vector3D(-0.5f, 0.5f, 0.5f),	Vector2D(0.f, 0.f),	Vector3D(-1.f, 0.f, 0.f), },	// 7	16
		{ Vector3D(-0.5f, 0.5f,-0.5f),	Vector2D(1.f, 0.f),	Vector3D(-1.f, 0.f, 0.f), },	// 4	17
		{ Vector3D(-0.5f,-0.5f,-0.5f),	Vector2D(1.f, 1.f),	Vector3D(-1.f, 0.f, 0.f), },	// 0	18
		{ Vector3D(-0.5f,-0.5f, 0.5f),	Vector2D(0.f, 1.f),	Vector3D(-1.f, 0.f, 0.f), },	// 3	19

		{ Vector3D(-0.5f, 0.5f, 0.5f),	Vector2D(0.f, 0.f),	Vector3D(0.f, 1.f, 0.f), },	// 7	20
		{ Vector3D(0.5f, 0.5f, 0.5f),	Vector2D(1.f, 0.f),	Vector3D(0.f, 1.f, 0.f), },	// 6	21
		{ Vector3D(0.5f, 0.5f,-0.5f),	Vector2D(1.f, 1.f),	Vector3D(0.f, 1.f, 0.f), },	// 5	22
		{ Vector3D(-0.5f, 0.5f,-0.5f),	Vector2D(0.f, 1.f),	Vector3D(0.f, 1.f, 0.f), },	// 4	23
	};

	// ���۸� �����ϴ� ����ü
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(cube);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ����. �ؽ�ó ������ �Ҷ��� ����
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = cube;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// ���� ������ ���� ������ �������� ���۸� �ʱ�ȭ��.
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
		0, 1, 2,		// ��
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23,
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
	ComPtr<ID3D11ShaderResourceView> textureView;				// ../ ���� ��δ� �ٽ� �����غ���.. �������� ���� �� ���� ������ ����...
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

	axis = new Axis(m_pd3dDevice, m_pd3dDeviceContext, m_wireRasterizerState);
	if (axis == nullptr)
	{
		return S_FALSE;
	}
	axis->Initialize();

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
	grid->Initialize();

	return hr;
}

void DXGraphics::BeginDraw()
{
	// ���� Ÿ�� ����.
	m_pd3dDeviceContext->OMSetRenderTargets(
		1,
		m_pd3dRenderTargetView.GetAddressOf(),
		m_pd3dDepthStencilView.Get()
	);

	// ���� Ÿ�� �� Ŭ����
	m_pd3dDeviceContext->ClearRenderTargetView(
		m_pd3dRenderTargetView.Get(), 
		Color::Black
	);

	// ���� ���ٽ� �� Ŭ����
	m_pd3dDeviceContext->ClearDepthStencilView(
		m_pd3dDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
		1.0f, 
		0
	);

	Matrix4x4 wvp = camera.GetViewProjMatrix();
	DirectX::XMMATRIX worldViewProj = ConvertToXMMATRIX(wvp);

	/// ���̴� �ڵ� �ڸ�?? �ε�??	

	grid->Render();
	axis->Render();

	{
		// �����Ͷ����� ������Ʈ
		m_pd3dDeviceContext->RSSetState(m_currRasterizerState.Get());

		m_pd3dDeviceContext->IASetInputLayout(m_cubeInputLayout.Get());
		m_pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// IA�� ���ؽ� ���� ����
		UINT stride = sizeof(PTNVertex);
		UINT offset = 0;
		m_pd3dDeviceContext->IASetVertexBuffers(
			0,
			1,
			m_cubeVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		// IA�� �ε��� ���� ����
		m_pd3dDeviceContext->IASetIndexBuffer(
			m_cubeIndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT,					// 32��Ʈ unsigned int ������ ����
			0
		);

		// ��� ���� ����
		m_cubeMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		float light[3] = { directionalLight.x, directionalLight.y, directionalLight.z };
		m_directionalLight->SetFloatVector(light);
		m_cubeShaderResource->SetResource(m_cubeTextureView.Get());

		// ��ũ��
		D3DX11_TECHNIQUE_DESC techDesc;
		m_cubeTechnique->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_cubeTechnique->GetPassByIndex(p)->Apply(0, m_pd3dDeviceContext.Get());
			
			m_pd3dDeviceContext->DrawIndexed(cubeIndexCount, 0, 0);
		}
	}
}

void DXGraphics::EndDraw()
{
	m_pDXGISwapChain1->Present(1, 0);
}

 