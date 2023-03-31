#include "DXGraphics.h"

#include <fstream>
#include <vector>

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
		return hr;
	}

	hr = CreateSwapChain();
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

	hr = CreateRasterState();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateObject();
	hr = CreateShaders();

	// Test();

	return hr;
}

void DXGraphics::Finalize()
{

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

	D3D11_RASTERIZER_DESC rasterDesc;				// 채우는 모드
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.FillMode = D3D11_FILL_SOLID;			// 채우기 모드
	rasterDesc.CullMode = D3D11_CULL_BACK;			// 지정된 방향은 그리지 않음 (BACK이니 뒷면은 그리지 않음)	
	rasterDesc.FrontCounterClockwise = false;		// 시계방향으로 할 거임
	rasterDesc.DepthClipEnable = true;				// 거리에 따라 클리핑을 할지

	hr = m_pd3dDevice->CreateRasterizerState(
		&rasterDesc, 
		m_solidRasterizerState.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_RASTERIZER_DESC Desc;						// 채우지 않는 모드
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;		// 채우기 모드
	rasterDesc.CullMode = D3D11_CULL_BACK;			// 지정된 방향은 그리지 않음 (BACK이니 뒷면은 그리지 않음)	
	rasterDesc.FrontCounterClockwise = false;		// 시계방향으로 할 거임
	rasterDesc.DepthClipEnable = true;				// 거리에 따라 클리핑을 할지

	hr = m_pd3dDevice->CreateRasterizerState(
		&rasterDesc,
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

	hr = DrawGrid();
	if (FAILED(hr))
	{
		return hr;
	}
}

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

HRESULT DXGraphics::CreateShaders()
{
	HRESULT hr = S_OK;

	std::ifstream fin("CubeVertexShader.cso", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> vsCompiledShader(size);

	fin.read(&vsCompiledShader[0], size);
	fin.close();

	ComPtr<ID3D11VertexShader> vertexShader;
	hr = m_pd3dDevice->CreateVertexShader(
		&vsCompiledShader[0], 
		size, 
		0, 
		vertexShader.GetAddressOf()
	);

	D3D11CreateEffectFromMemory();
	m_effectTechnique = m_effect->GetTechniqueByName()

	if (FAILED(hr))
	{
		return hr;
	}

	vertexShader.As(&m_vertexShader);

	D3D11_INPUT_ELEMENT_DESC iaDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = m_pd3dDevice->CreateInputLayout(
		iaDesc, 
		ARRAYSIZE(iaDesc), 
		&vsCompiledShader[0],
		size, 
		m_pd3dInputLayout.GetAddressOf());

	if (FAILED(hr))
	{
		return hr;
	}


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

	return hr;
}

HRESULT DXGraphics::DrawGrid()
{
	HRESULT hr = S_OK;

	// 꼭짓점을 설명하는 정보
	VertexCombined grid[] =
	{
// 		{ DirectX::XMFLOAT3(0.f, 0.f, 0.f), Color::White },		// 위치
// 		{ DirectX::XMFLOAT3(1.f, 0.f, 0.f), Color::White },
// 		{ DirectX::XMFLOAT3(0.f, 0.f, 1.f), Color::White },
// 		{ DirectX::XMFLOAT3(1.f, 0.f, 1.f), Color::White },
// 
// 		{ DirectX::XMFLOAT3(2.f, 0.f, 0.f), Color::White },
// 		{ DirectX::XMFLOAT3(2.f, 0.f, 1.f), Color::White },
// 		{ DirectX::XMFLOAT3(0.f, 0.f, 2.f), Color::White },
// 		{ DirectX::XMFLOAT3(1.f, 0.f, 2.f), Color::White },
// 		{ DirectX::XMFLOAT3(2.f, 0.f, 2.f), Color::White }

		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(0,   0,   0),},
		{DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(0,   0,   1),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(0,   1,   0),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0,   1,   1),},

		{DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(1,   0,   0),},
		{DirectX::XMFLOAT3(0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(1,   0,   1),},
		{DirectX::XMFLOAT3(0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(1,   1,   0),},
		{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1,   1,   1),},
	};

	// 버퍼를 설정하는 구조체
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(grid);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// 하위 리소스 설정. 텍스처 같은거 할때나 쓰임
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = grid;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// 위에 설정한 버퍼 설정을 바탕으로 버퍼를 초기화함.
	hr = m_pd3dDevice->CreateBuffer(
		&bufferDesc, 
		&InitData, 
		m_vertexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	UINT indices[] = { 
// 		0,1,2, 
// 		2,1,3, 
// 		1,4,3, 
// 		3,4,5, 
// 		2,3,6, 
// 		6,3,7, 
// 		3,5,7, 
// 		7,5,8 

		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	count = ARRAYSIZE(indices);

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
		m_indexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void DXGraphics::BeginDraw()
{
	m_pd3dDeviceContext->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		nullptr,
		&m_constantBufferData,
		0,
		0
	);

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

	m_pd3dDeviceContext->RSSetState(0);
	

	/// 쉐이더 코드 자리?? 인듯??
	UINT stride = sizeof(VertexCombined);
	UINT offset = 0;
	m_pd3dDeviceContext->IASetVertexBuffers(
		0, 
		1, 
		m_vertexBuffer.GetAddressOf(), 
		&stride, 
		&offset
	);

	m_pd3dDeviceContext->IASetIndexBuffer(
		m_indexBuffer.Get(), 
		DXGI_FORMAT_R32_UINT,					// 32비트 unsigned int 형으로 읽음
		0
	);

	m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dDeviceContext->IASetInputLayout(m_pd3dInputLayout.Get());
	
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

