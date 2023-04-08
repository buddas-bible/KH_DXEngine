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

	// ������ ī�޶� �����ǿ� ī�޶� ������ ������ �Ϳ� -�� �ٿ���� ������
	// �� ��Ŀ� ���Ƿ� �־��ֱ�� ����.
	m_constantBufferData.view.r[3].m128_f32[0] = -m_pos.x;
	m_constantBufferData.view.r[3].m128_f32[1] = -m_pos.y;
	m_constantBufferData.view.r[3].m128_f32[2] = -m_pos.z;
	m_constantBufferData.view.r[3].m128_f32[3] = 1.f;
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

	m_pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);

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

		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	// ���ؽ� ������ ��� �Ǿ��ִ��� ����
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
		ARRAYSIZE(iaDesc),						// ���ؽ��� �� ������ ����
		passDesc.pIAInputSignature,				// ���̴� �ڵ� ������
		passDesc.IAInputSignatureSize,			// ���̴� ũ��
		m_axisInputLayout.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}
	
	m_gridTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = m_pd3dDevice->CreateInputLayout(
		iaDesc,
		2,										// ���ؽ��� �� ������ ����
		passDesc.pIAInputSignature,				// ���̴� �ڵ� ������
		passDesc.IAInputSignatureSize,			// ���̴� ũ��
		m_gridInputLayout.GetAddressOf()
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
		L"./VertexShader.fx",					// ���̴� ���� �̸�
		0,										// ���̴� ��ũ��
		0,										//
		0,										// ������
		"fx_5_0",								// ���̴� ���
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

	// �� ����Ʈ
	{
		hr = D3DX11CreateEffectFromMemory(
			&vsCompiledShader[0],
			cSize,
			0,
			m_pd3dDevice.Get(),
			m_axisEffect.GetAddressOf()
		);

		m_axisTechnique = m_axisEffect->GetTechniqueByName("Tech");							// ���Ͽ� Tech �̸��� �����͸� �о��
		m_axisMatrixVariable = m_axisEffect->GetVariableByName("worldViewProj")->AsMatrix();	// ���Ͽ� worldViewProj �̸��� �����͸� �о��
	}

	if (FAILED(hr))
	{
		return hr;
	}

	// �׸��� ����Ʈ
	{
		hr = D3DX11CreateEffectFromMemory(
			&vsCompiledShader[0],
			size,
			0,
			m_pd3dDevice.Get(),
			m_gridEffect.GetAddressOf()
		);

		m_gridTechnique = m_gridEffect->GetTechniqueByName("Tech");							// ���Ͽ� Tech �̸��� �����͸� �о��
		m_gridMatrixVariable = m_gridEffect->GetVariableByName("worldViewProj")->AsMatrix();	// ���Ͽ� worldViewProj �̸��� �����͸� �о��
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

	/// ������ ����� ��Ʈ������ ����������
	/// ������ ������Ʈ �Ǵ� ��ҷ� ������ ��
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, -1.5f, 1.f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.1f, 0.0f, 1.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.f);

	m_pos = { 0.0f, 0.7f, -1.5f, 1.f };

	// ���� ���
	m_constantBufferData.world = DirectX::XMMatrixIdentity();

	// �� ��Ʈ����
	m_constantBufferData.view = 
		DirectX::XMMatrixLookAtLH(
			eye,							// ī�޶� ��ġ
			at,								// ī�޶� ����
			up								// ī�޶� ������
		);

	// ����ۿ��� ������ ������
	D3D11_TEXTURE2D_DESC backBufferDesc{};
	ZeroMemory(&backBufferDesc, sizeof(backBufferDesc));
	m_backBuffer->GetDesc(&backBufferDesc);

	// ȭ�� ����
	float aspectRatioX = static_cast<float>(backBufferDesc.Width) / static_cast<float>(backBufferDesc.Height);
	float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

	// ���� ��Ʈ����
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

	/// ��� ���� �����ϱ� ���� ��
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

	// �������� �����ϴ� ����
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
			0, 1, 3,		// ��
			3, 1, 2,

			4, 5, 0,		// ��
			0, 5, 1,

			7, 4, 3,		// ��
			3, 4, 0,

			6, 7, 2,		// �� 
			2, 7, 3,

			5, 6, 1,		// ��
			1, 6, 2,

			7, 6, 5,		// ��
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
	
	// �������� �����ϴ� ����
	Vertex axis[] =
	{
		{DirectX::XMFLOAT3(0.f, 0.f, 0.f),	DirectX::XMFLOAT4(Color::Red),},
		{DirectX::XMFLOAT3(15.f, 0.f, 0.f), DirectX::XMFLOAT4(Color::Red),},

		{DirectX::XMFLOAT3(0.f, 0.f, 0.f),	DirectX::XMFLOAT4(Color::Green),},
		{DirectX::XMFLOAT3(0.f ,15.f, 0.f), DirectX::XMFLOAT4(Color::Green),},

		{DirectX::XMFLOAT3(0.f, 0.f, 0.f),	DirectX::XMFLOAT4(Color::Blue),},
		{DirectX::XMFLOAT3(0.f ,0.f, 15.f), DirectX::XMFLOAT4(Color::Blue),},
	};

	// ���۸� �����ϴ� ����ü
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(axis);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ����. �ؽ�ó ������ �Ҷ��� ����
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = axis;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// ���� ������ ���� ������ �������� ���۸� �ʱ�ȭ��.
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
	
	// �������� �����ϴ� ����
	Vertex vertices[100];
	for (int i = 0; i < 100; i++)
	{
		vertices[i].pos = DirectX::XMFLOAT3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
		vertices[i].color = DirectX::XMFLOAT4(Color::White);
	}

	// ���۸� �����ϴ� ����ü
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ����. �ؽ�ó ������ �Ҷ��� ����
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// ���� ������ ���� ������ �������� ���۸� �ʱ�ȭ��.
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
	// ���� Ÿ�� ����.
	m_pd3dDeviceContext->OMSetRenderTargets(
		1,
		m_pd3dRenderTargetView.GetAddressOf(),
		m_pd3dDepthStencilView.Get()
	);

	// ���� Ÿ�� �� Ŭ����
	m_pd3dDeviceContext->ClearRenderTargetView(
		m_pd3dRenderTargetView.Get(), 
		Color::SaddleBrown
	);

	// ���� ���ٽ� �� Ŭ����
	m_pd3dDeviceContext->ClearDepthStencilView(
		m_pd3dDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
		1.0f, 
		0
	);


	/// ���̴� �ڵ� �ڸ�?? �ε�??	
	{
		// �����Ͷ����� ������Ʈ
		m_pd3dDeviceContext->RSSetState(m_wireRasterizerState.Get());

		m_pd3dDeviceContext->IASetInputLayout(m_axisInputLayout.Get());
		m_pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		// IA�� ���ؽ� ���� ����
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_pd3dDeviceContext->IASetVertexBuffers(
			0,
			1,
			m_axisVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		// IA�� �ε��� ���� ����
		m_pd3dDeviceContext->IASetIndexBuffer(
			m_axisIndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT,					// 32��Ʈ unsigned int ������ ����
			0
		);

		// ��� ���� ����
		DirectX::XMMATRIX worldViewProj = m_constantBufferData.world * m_constantBufferData.view * m_constantBufferData.projection;
		m_axisMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		// ��ũ��
		D3DX11_TECHNIQUE_DESC techDesc;
		m_axisTechnique->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_axisTechnique->GetPassByIndex(p)->Apply(0, m_pd3dDeviceContext.Get());

			m_pd3dDeviceContext->DrawIndexed(axisIndexCount, 0, 0);
		}
	}	

	{
		// �����Ͷ����� ������Ʈ
		m_pd3dDeviceContext->RSSetState(m_wireRasterizerState.Get());

		m_pd3dDeviceContext->IASetInputLayout(m_gridInputLayout.Get());
		m_pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		// IA�� ���ؽ� ���� ����
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_pd3dDeviceContext->IASetVertexBuffers(
			0,
			1,
			m_gridVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		// IA�� �ε��� ���� ����
		m_pd3dDeviceContext->IASetIndexBuffer(
			m_gridIndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT,					// 32��Ʈ unsigned int ������ ����
			0
		);

		// ��� ���� ����
		DirectX::XMMATRIX worldViewProj = m_constantBufferData.world * m_constantBufferData.view * m_constantBufferData.projection;
		m_gridMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		// ��ũ��
		D3DX11_TECHNIQUE_DESC techDesc;
		m_gridTechnique->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_gridTechnique->GetPassByIndex(p)->Apply(0, m_pd3dDeviceContext.Get());

			m_pd3dDeviceContext->DrawIndexed(gridIndexCount, 0, 0);
		}
	}

	{
		// �����Ͷ����� ������Ʈ
		m_pd3dDeviceContext->RSSetState(m_currRasterizerState.Get());

		m_pd3dDeviceContext->IASetInputLayout(m_cubeInputLayout.Get());
		m_pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// IA�� ���ؽ� ���� ����
		UINT stride = sizeof(TextureVertex);
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
		DirectX::XMMATRIX worldViewProj = m_constantBufferData.world * m_constantBufferData.view * m_constantBufferData.projection;
		m_cubeMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		// m_cubeSampler->SetSampler(0, m_cubeSamplerState.Get());
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
	
	/// ����ü��1�� ������Ʈ1�� ����ϱ� ���ؼ��� �̰͵� �ʿ���.
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

 