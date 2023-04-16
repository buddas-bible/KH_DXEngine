#include "Skull.h"

#include <fstream>
#include <vector>

Skull::~Skull()
{

}

HRESULT Skull::Initialize()
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

/// <summary>
/// 교수님 예제를 가져다 씀
/// 
/// 최대한 코드를 이해해보려고 함
/// </summary>
/// <returns></returns>
HRESULT Skull::CreateBuffer()
{
	HRESULT hr = S_OK;

	// 텍스트 파일에서 버텍스 정보를 읽어온다.
	std::ifstream fin("Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return S_FALSE;
	}

	UINT vcount = 0;			// 파일에 명시되어있는 버텍스 개수를 저장하는 변수
	UINT tcount = 0;			// 파일에 명시되어있는 삼각형 개수를 저장하는 변수
	std::string ignore;			// 필요없는 데이터를 치워주는 스트링

	fin >> ignore >> vcount;	// 버텍스 문자열을 이그노어에 치워버리고 버텍스 개수만 취함
	fin >> ignore >> tcount;	// 삼각형 문자열을 이그노어에 치워버리고 삼각형 개수만 취함
	fin >> ignore >> ignore >> ignore >> ignore; // 중괄호 등 필요없는 문자를 치워버림

	std::vector<PNVertex> vertices(vcount);	// 버텍스 개수만큼 벡터 크기를 정함
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;

		fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
	}

	fin >> ignore;		// 버텍스와 인덱스 중간에 필요없는 중괄호 없애는 용도
	fin >> ignore;		// ''
	fin >> ignore;		// ''

	count = 3 * tcount;		// 인덱스는 삼각형을 * 3
	std::vector<UINT> indices(count);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(PNVertex) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	vinitData.SysMemPitch = 0;
	vinitData.SysMemSlicePitch = 0;
	
	hr = m_device->CreateBuffer(
		&vbd, 
		&vinitData, 
		m_vertexBuffer.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * count;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	iinitData.SysMemPitch = 0;
	iinitData.SysMemSlicePitch = 0;

	hr = m_device->CreateBuffer(
		&ibd,
		&iinitData,
		m_indexBuffer.GetAddressOf()
	);

	return hr;
}

HRESULT Skull::CreateLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC textureDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// 인풋 레이아웃 만듬
	D3DX11_PASS_DESC passDesc;
	m_tech->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = m_device->CreateInputLayout(
		textureDesc,
		ARRAYSIZE(textureDesc),					// 버텍스에 들어간 데이터 갯수
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

HRESULT Skull::CreateEffect()
{
	HRESULT hr = S_OK;

#if _WIN64

#if _DEBUG
	std::ifstream fin("../x64/debug/WhireWire.cso", std::ios::binary);
#else
	std::ifstream fin("../x64/release/WhireWire.cso", std::ios::binary);		// 이부분 경로 다시 생각해보자...
#endif

#else

#if _DEBUG
	std::ifstream fin("../WIN32/debug/WhireWire.cso", std::ios::binary);
#else
	std::ifstream fin("../WIN32/release/WhireWire.cso", std::ios::binary);
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
	m_light = m_effect->GetVariableByName("lightDirection");


	return hr;
}

HRESULT Skull::Finalize()
{
	return S_OK;
}

void Skull::Update(const Matrix4x4& view, const Matrix4x4& proj)
{
	m_worldTM = CreateMatrix(m_pos, m_angle, m_scale);
	m_viewTM = view;
	m_projTM = proj;
}

void Skull::Render()
{
	// 레스터라이즈 스테이트
	m_context->RSSetState(m_randerState.Get());

	m_context->IASetInputLayout(m_inputLayout.Get());
	m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// IA에 버텍스 버퍼 설정
	UINT stride = sizeof(PNVertex);
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

	// 상수 버퍼 설정
	m_matrix->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	const float light[3] = { 0.5f, 0.5f, 0.f };
	m_light->SetRawValue(light, 0, sizeof(light));

	// 테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply(0, m_context.Get());

		m_context->DrawIndexed(count, 0, 0);
	}
}
