#include "Box.h"

#include <fstream>
#include <vector>

using namespace Microsoft;
using namespace Microsoft::WRL;

Box::~Box()
{

}

HRESULT Box::Initialize()
{
	HRESULT hr = S_OK;

	hr = CreateBuffer();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateEffect();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateLayout();
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT Box::CreateBuffer()
{
	HRESULT hr = S_OK;

	// 꼭짓점을 설명하는 정보
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
	hr = m_device->CreateBuffer(
		&bufferDesc,
		&InitData,
		m_vertexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	UINT indices[] =
	{
		0, 1, 2,		// 하
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

	hr = m_device->CreateBuffer(
		&indexBufferDesc,
		&indexInit,
		m_indexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	ComPtr<ID3D11Resource> texture;
	ComPtr<ID3D11ShaderResourceView> textureView;				// ../ 같은 경로는 다시 생각해보자.. 실행파일 뽑을 때 많이 귀찮아 진다...
	hr = DirectX::CreateDDSTextureFromFile(m_device.Get(), L"../Textures/WoodCrate01.dds", texture.GetAddressOf(), textureView.GetAddressOf());
	// hr = DirectX::CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"WoodCrate01.dds", texture.GetAddressOf(), textureView.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	texture.As(&m_texture);
	textureView.As(&m_textureView);

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
	hr = m_device->CreateSamplerState(&samplerDesc, samplerstate.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	samplerstate.As(&m_samplerState);

	return hr;
}

HRESULT Box::CreateLayout()
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
	D3DX11_PASS_DESC passDesc;
	m_tech->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = m_device->CreateInputLayout(
		textureDesc,
		ARRAYSIZE(textureDesc),						// 버텍스에 들어간 데이터 갯수
		passDesc.pIAInputSignature,				// 셰이더 코드 포인터
		passDesc.IAInputSignatureSize,			// 셰이더 크기
		m_inputLayout.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT Box::CreateEffect()
{
	HRESULT hr = S_OK;

#if _WIN64

#if _DEBUG
	std::ifstream fin("../x64/debug/Texture.cso", std::ios::binary);
#else
	std::ifstream fin("../x64/release/Texture.cso", std::ios::binary);		// 이부분 경로 다시 생각해보자...
#endif

#else

#if _DEBUG
	std::ifstream fin("../WIN32/debug/Texture.cso", std::ios::binary);
#else
	std::ifstream fin("../WIN32/release/Texture.cso", std::ios::binary);
#endif

#endif

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> vsCompiledShader(size);

	fin.read(&vsCompiledShader[0], size);
	fin.close();

	hr = D3DX11CreateEffectFromMemory(
		&vsCompiledShader[0],
		size,
		0,
		m_device.Get(),
		m_effect.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	m_tech = m_effect->GetTechniqueByName("Tech");							// 파일에 Tech 이름의 데이터를 읽어옴
	m_matrix = m_effect->GetVariableByName("worldViewProj")->AsMatrix();	// 파일에 worldViewProj 이름의 데이터를 읽어옴
	m_invMatrix = m_effect->GetVariableByName("invTworldViewProj")->AsMatrix();	// 파일에 worldViewProj 이름의 데이터를 읽어옴

	// 이펙트로 셰이더 리소스랑 샘플러가 필요하지 않을까?
	m_shaderResource = m_effect->GetVariableByName("g_Texture")->AsShaderResource();	// 
	m_sampler = m_effect->GetVariableByName("g_Sampler")->AsSampler();

	m_directionalLight = m_effect->GetVariableByName("lightDirection")->AsVector();
	// m_Light = m_effect->GetVariableByName("pLight")->AsVector();

	return hr;
}

HRESULT Box::Finalize()
{
	HRESULT hr = S_OK;

	return hr;
}

void Box::Update(const Matrix4x4& view, const Matrix4x4& proj)
{
	if (GetAsyncKeyState('I'))
	{
		m_pos.z += 0.01f;
	}
	if (GetAsyncKeyState('K'))
	{
		m_pos.z -= 0.01f;
	}
	if (GetAsyncKeyState('J'))
	{
		m_angle.z -= 0.01f;
	}
	if (GetAsyncKeyState('L'))
	{
		m_angle.z += 0.01f;
	}

	m_worldTM = CreateMatrix(m_pos, m_angle, m_scale);
	invT = TransposeMatrix(CreateInvMatrix(m_pos, m_angle, m_scale));

	m_viewTM = view;
	m_projTM = proj;
}

void Box::Render()
{
	// 레스터라이즈 스테이트
	m_context->RSSetState(m_randerState.Get());

	m_context->IASetInputLayout(m_inputLayout.Get());
	m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// IA에 버텍스 버퍼 설정
	UINT stride = sizeof(PTNVertex);
	UINT offset = 0;
	m_context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	// IA에 인덱스 버퍼 설정
	m_context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,					// 32비트 unsigned int 형으로 읽음
		0
	);

	Matrix4x4 wvp = m_worldTM * m_viewTM * m_projTM;
	DirectX::XMMATRIX worldViewProj = ConvertToXMMATRIX(wvp);
	Matrix4x4 invMatrix = CreateInvMatrix(m_pos, m_angle, m_scale);
	DirectX::XMMATRIX inv = ConvertToXMMATRIX(invT);

	// 상수 버퍼 설정
	m_matrix->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
	m_invMatrix->SetMatrix(reinterpret_cast<float*>(&inv));
	const float light[3] = { 0.5f, 0.5f, 0.f };
	m_directionalLight->SetRawValue(light, 0, sizeof(light));

	m_shaderResource->SetResource(m_textureView.Get());

	// 테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply(0, m_context.Get());

		m_context->DrawIndexed(count, 0, 0);
	}
}
