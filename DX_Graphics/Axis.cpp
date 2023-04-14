#include "Axis.h"

#include <fstream>
#include <vector>

Axis::~Axis()
{

}

HRESULT Axis::Initialize()
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

HRESULT Axis::CreateBuffer()
{
	HRESULT hr = S_OK;

	// �������� �����ϴ� ����
	PCVertex axis[] =
	{
		{Vector3D(0.f, 0.01f, 0.f),	Vector4D(Color::Red),},
		{Vector3D(15.f, 0.01f, 0.f), Vector4D(Color::Red),},

		{Vector3D(0.f, 0.f, 0.f),	Vector4D(Color::Green),},
		{Vector3D(0.f ,15.f, 0.f), Vector4D(Color::Green),},

		{Vector3D(0.f, 0.01f, 0.f),	Vector4D(Color::Blue),},
		{Vector3D(0.f ,0.01f, 15.f), Vector4D(Color::Blue),},
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
	hr = m_device->CreateBuffer(
		&bufferDesc,
		&InitData,
		m_vertexBuffer.GetAddressOf()
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

	return hr;
}

HRESULT Axis::CreateLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC iaDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	D3DX11_PASS_DESC passDesc;
	m_tech->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = m_device->CreateInputLayout(
		iaDesc,
		ARRAYSIZE(iaDesc),						// ���ؽ��� �� ������ ����
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

HRESULT Axis::CreateEffect()
{
	HRESULT hr = S_OK;

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

	hr = D3DX11CreateEffectFromMemory(
		&vsCompiledShader[0],
		cSize,
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

	return hr;
}

HRESULT Axis::Finalize()
{
	return S_OK;
}

void Axis::Update(const Matrix4x4& view, const Matrix4x4& proj)
{
	m_viewTM = view;
	m_projTM = proj;
}

void Axis::Render()
{
	// �����Ͷ����� ������Ʈ
	m_context->RSSetState(m_randerState.Get());

	m_context->IASetInputLayout(m_inputLayout.Get());
	m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// IA�� ���ؽ� ���� ����
	UINT stride = sizeof(PCVertex);
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

	// ��ũ��
	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply(0, m_context.Get());

		m_context->DrawIndexed(count, 0, 0);
	}
}
