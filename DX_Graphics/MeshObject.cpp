#include "MeshObject.h"

#include <fstream>
#include <vector>
#include "CParsingDataClass.h"

using namespace Microsoft;
using namespace Microsoft::WRL;

MeshObject::~MeshObject()
{

}

HRESULT MeshObject::Initialize()
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

HRESULT MeshObject::CreateBuffer()
{

	return S_OK;
}

HRESULT MeshObject::CreateLayout()
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

HRESULT MeshObject::CreateEffect()
{
	HRESULT hr = S_OK;

#if _WIN64

#if _DEBUG
	std::ifstream fin("../x64/debug/NonTextureBasic.cso", std::ios::binary);
#else
	std::ifstream fin("../x64/release/NonTextureBasic.cso", std::ios::binary);		// 이부분 경로 다시 생각해보자...
#endif

#else

#if _DEBUG
	std::ifstream fin("../WIN32/debug/NonTextureBasic.cso", std::ios::binary);
#else
	std::ifstream fin("../WIN32/release/NonTextureBasic.cso", std::ios::binary);
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

	if (isTexture)
	{
		m_shaderResource = m_effect->GetVariableByName("g_Texture")->AsShaderResource();	// 
		m_sampler = m_effect->GetVariableByName("g_Sampler")->AsSampler();
	}

	return hr;
}

HRESULT MeshObject::Finalize()
{
	return S_OK;

}

void MeshObject::Update(const Matrix4x4& view, const Matrix4x4& proj)
{
	m_worldTM = CreateMatrix(m_pos, m_angle, m_scale);
	m_viewTM = view;
	m_projTM = proj;
}

void MeshObject::Render()
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

	// 상수 버퍼 설정
	m_matrix->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	const float light[3] = { 0.5f, 0.5f, 0.f };
	m_light->SetRawValue(light, 0, sizeof(light));

	if (isTexture)
	{
		m_shaderResource->SetResource(m_textureView.Get());
	}

	// 테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply(0, m_context.Get());

		m_context->DrawIndexed(count, 0, 0);
	}
}

HRESULT MeshObject::LoadGeometry(ASEParser::Mesh* meshData)
{
	HRESULT hr = S_OK;

	UINT vcount = 0;
	UINT tcount = 0;

	vcount = meshData->m_meshvertex.size();
	std::vector<PTNVertex> vertices(vcount);

	//for (ASEParser::Vertex* vertex : meshData->m_meshvertex)
	for (int i = 0; i < vcount; i++)
	{
		vertices[i].pos.x = meshData->m_meshvertex[i]->m_pos.x;
		vertices[i].pos.y = meshData->m_meshvertex[i]->m_pos.y;
		vertices[i].pos.z = meshData->m_meshvertex[i]->m_pos.z;

		// vertices[i].uv.x = meshData->m_meshvertex[i]->u;
		// vertices[i].uv.y = meshData->m_meshvertex[i]->v;
		if (meshData->m_mesh_tvertex.size() != 0)
		{
			vertices[i].uv.x = meshData->m_mesh_tvertex[i]->m_u;
			vertices[i].uv.y = meshData->m_mesh_tvertex[i]->m_v;
		}

		vertices[i].normal.x = meshData->m_meshvertex[i]->m_normal.x;
		vertices[i].normal.y = meshData->m_meshvertex[i]->m_normal.y;
		vertices[i].normal.z = meshData->m_meshvertex[i]->m_normal.z;
	}

	tcount = meshData->m_mesh_numfaces;

	count = 3 * tcount;
	std::vector<UINT> indices(count);
	for (UINT i = 0; i < tcount; ++i)
	{
		indices[i * 3 + 0] = meshData->m_meshface[i]->m_vertexindex[0];
		indices[i * 3 + 1] = meshData->m_meshface[i]->m_vertexindex[2];
		indices[i * 3 + 2] = meshData->m_meshface[i]->m_vertexindex[1];
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(PTNVertex) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	hr = m_device->CreateBuffer(&vbd, &vinitData, &m_vertexBuffer);

	if (FAILED(hr))
	{
		return hr;
	}

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * count;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	hr = m_device->CreateBuffer(&ibd, &iinitData, &m_indexBuffer);

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void MeshObject::SetScalse(Vector3D scale)
{
	m_scale = scale;
}

void MeshObject::SetPosition(Vector3D position)
{
	m_pos = position;
}

HRESULT MeshObject::SetTexture(const wstring& path)
{
	HRESULT hr = S_OK;

	isTexture = true;
	ComPtr<ID3D11Resource> texture;
	ComPtr<ID3D11ShaderResourceView> textureView;				// ../ 같은 경로는 다시 생각해보자.. 실행파일 뽑을 때 많이 귀찮아 진다...
	hr = DirectX::CreateDDSTextureFromFile(m_device.Get(), path.c_str(), texture.GetAddressOf(), textureView.GetAddressOf());
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
