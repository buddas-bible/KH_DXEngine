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
/// ������ ������ ������ ��
/// 
/// �ִ��� �ڵ带 �����غ����� ��
/// </summary>
/// <returns></returns>
HRESULT Skull::CreateBuffer()
{
	HRESULT hr = S_OK;

	// �ؽ�Ʈ ���Ͽ��� ���ؽ� ������ �о�´�.
	std::ifstream fin("Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return S_FALSE;
	}

	UINT vcount = 0;			// ���Ͽ� ��õǾ��ִ� ���ؽ� ������ �����ϴ� ����
	UINT tcount = 0;			// ���Ͽ� ��õǾ��ִ� �ﰢ�� ������ �����ϴ� ����
	std::string ignore;			// �ʿ���� �����͸� ġ���ִ� ��Ʈ��

	fin >> ignore >> vcount;	// ���ؽ� ���ڿ��� �̱׳� ġ�������� ���ؽ� ������ ����
	fin >> ignore >> tcount;	// �ﰢ�� ���ڿ��� �̱׳� ġ�������� �ﰢ�� ������ ����
	fin >> ignore >> ignore >> ignore >> ignore; // �߰�ȣ �� �ʿ���� ���ڸ� ġ������

	std::vector<PNVertex> vertices(vcount);	// ���ؽ� ������ŭ ���� ũ�⸦ ����
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;

		fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
	}

	fin >> ignore;		// ���ؽ��� �ε��� �߰��� �ʿ���� �߰�ȣ ���ִ� �뵵
	fin >> ignore;		// ''
	fin >> ignore;		// ''

	count = 3 * tcount;		// �ε����� �ﰢ���� * 3
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

	// ��ǲ ���̾ƿ� ����
	D3DX11_PASS_DESC passDesc;
	m_tech->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = m_device->CreateInputLayout(
		textureDesc,
		ARRAYSIZE(textureDesc),					// ���ؽ��� �� ������ ����
		passDesc.pIAInputSignature,				// ���̴� �ڵ� ������
		passDesc.IAInputSignatureSize,			// ���̴� ũ��
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
	std::ifstream fin("../x64/release/WhireWire.cso", std::ios::binary);		// �̺κ� ��� �ٽ� �����غ���...
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

	m_tech = m_effect->GetTechniqueByName("Tech");							// ���Ͽ� Tech �̸��� �����͸� �о��
	m_matrix = m_effect->GetVariableByName("worldViewProj")->AsMatrix();	// ���Ͽ� worldViewProj �̸��� �����͸� �о��
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
	// �����Ͷ����� ������Ʈ
	m_context->RSSetState(m_randerState.Get());

	m_context->IASetInputLayout(m_inputLayout.Get());
	m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// IA�� ���ؽ� ���� ����
	UINT stride = sizeof(PNVertex);
	UINT offset = 0;
	m_context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	// IA�� �ε��� ���� ����
	m_context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,					// 32��Ʈ unsigned int ������ ����
		0
	);

	Matrix4x4 wvp = m_worldTM * m_viewTM * m_projTM;
	DirectX::XMMATRIX worldViewProj = ConvertToXMMATRIX(wvp);

	// ��� ���� ����
	m_matrix->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	const float light[3] = { 0.5f, 0.5f, 0.f };
	m_light->SetRawValue(light, 0, sizeof(light));

	// ��ũ��
	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply(0, m_context.Get());

		m_context->DrawIndexed(count, 0, 0);
	}
}
