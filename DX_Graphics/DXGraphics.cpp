#include "DXGraphics.h"

#include <DirectXMath.h>

using namespace Microsoft::WRL;

DXGraphics::DXGraphics() : m_hWnd()
{

}

DXGraphics::~DXGraphics()
{

}

HRESULT DXGraphics::Initialize(HWND hwnd)
{
	m_hWnd = hwnd;

	HRESULT hr = S_OK;

	if (FAILED(CreateDevice()))
	{
		return hr;
	}
	if (FAILED(CreateSwapChain()))
	{
		return hr;
	}
	if (FAILED(CreateRenderTargetView()))
	{
		return hr;
	}
	if (FAILED(CreateDepthStencilView()))
	{
		return hr;
	}

	Test();

	return hr;
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
		&device,
		nullptr,
		&context);

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
		&factory2);

	// HWND�� ����ü�� ����
	ComPtr<IDXGISwapChain1> swapChain1;
	hr = factory2->CreateSwapChainForHwnd(
		m_pd3dDevice.Get(),
		m_hWnd,
		&dxgiSwapChainDesc1,
		NULL,
		NULL,
		&swapChain1);

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

	// m_pd3dRenderTargetView.Reset();

	// ����ü���� ������ �ִ� ���۸� ������
	ComPtr<ID3D11Texture2D> backBuffer;
	hr = m_pDXGISwapChain1->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		&backBuffer);

	if (FAILED(hr))
	{
		return hr;
	}

	backBuffer.As(&m_backBuffer);

	// ���� Ÿ������ ���۸� ����
	ComPtr<ID3D11RenderTargetView> rendertarget;
	hr = m_pd3dDevice->CreateRenderTargetView(
		m_backBuffer.Get(),
		nullptr,
		&rendertarget);

	if (FAILED(hr))
	{
		return hr;
	}

	rendertarget.As(&m_pd3dRenderTargetView);

	// ������� ����۷κ��� ������ �޾ƿ�
	D3D11_TEXTURE2D_DESC backBufferDesc{};
	ZeroMemory(&backBufferDesc, sizeof(backBufferDesc));
	m_backBuffer->GetDesc(&backBufferDesc);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.Width = static_cast<float>(backBufferDesc.Width);
	viewport.Height = static_cast<float>(backBufferDesc.Height);
	viewport.MinDepth = D3D11_MIN_DEPTH;
	viewport.MaxDepth = D3D11_MAX_DEPTH;

	m_pd3dDeviceContext->RSSetViewports(1, &viewport);

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

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	ComPtr<ID3D11DepthStencilView> depthStencilView;
	hr = m_pd3dDevice->CreateDepthStencilView(
		depthStencil.Get(),
		&depthStencilViewDesc,
		&depthStencilView);

	if (FAILED(hr))
	{
		return hr;
	}

	depthStencilView.As(&m_pd3dDepthStencilView);

	return hr;
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

void DXGraphics::BeginDraw()
{
	/*
	// ���� Ÿ�� ����.
	m_pd3dDeviceContext->OMSetRenderTargets(
		1,
		m_pd3dRenderTargetView.GetAddressOf(),
		m_pd3dDepthStencilView.Get());

	// ���� Ÿ�� �� Ŭ����
	const float clearColor[] = { 0.071f, 0.04f, 0.561f, 1.0f };
	m_pd3dDeviceContext->ClearRenderTargetView(
		m_pd3dRenderTargetView.Get(), 
		clearColor);

	// ���� ���ٽ� �� Ŭ����
	m_pd3dDeviceContext->ClearDepthStencilView(
		m_pd3dDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	*/

	/// ���̴� �ڵ� �ڸ�?? �ε�??


	/// ����ü��1�� ������Ʈ1�� ����ϱ� ���ؼ��� �̰͵� �ʿ���.
	// DXGI_PRESENT_PARAMETERS present;
	// present.DirtyRectsCount = 0;
	// present.pDirtyRects = nullptr;
	// present.pScrollRect = nullptr;
	// present.pScrollOffset = nullptr;
	// m_pDXGISwapChain1->Present1(1, 0, &present);

	Sleep(2000);
}

void DXGraphics::EndDraw()
{
	m_pDXGISwapChain1->Present(1, 0);
}

