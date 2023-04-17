#include "Teapot.h"

#include <fstream>
#include <vector>
#include "CParsingDataClass.h"

Teapot::~Teapot()
{

}

HRESULT Teapot::Initialize()
{
	return S_OK;
}

HRESULT Teapot::CreateBuffer()
{
	return S_OK;
}

HRESULT Teapot::CreateLayout()
{
	return S_OK;

}

HRESULT Teapot::CreateEffect()
{
	return S_OK;

}

HRESULT Teapot::Finalize()
{
	return S_OK;

}

void Teapot::Update(const Matrix4x4& view, const Matrix4x4& proj)
{
	m_viewTM = view;
	m_projTM = proj;
}

void Teapot::Render()
{

}

HRESULT Teapot::LoadGeometry(ASEParser::Mesh* meshData)
{
	HRESULT hr = S_OK;

	UINT vcount = 0;
	UINT tcount = 0;

	vcount = meshData->m_opt_vertex.size();
	std::vector<PTNVertex> vertices(vcount);

	//for (ASEParser::Vertex* vertex : meshData->m_meshvertex)
	for (int i = 0; i < vcount; i++)
	{
		vertices[i].pos.x = meshData->m_opt_vertex[i]->m_pos.x;
		vertices[i].pos.y = meshData->m_opt_vertex[i]->m_pos.y;
		vertices[i].pos.z = meshData->m_opt_vertex[i]->m_pos.z;

		vertices[i].normal.x = meshData->m_opt_vertex[i]->m_normal.x;
		vertices[i].normal.y = meshData->m_opt_vertex[i]->m_normal.y;
		vertices[i].normal.z = meshData->m_opt_vertex[i]->m_normal.z;

		vertices[i].uv.x = meshData->m_opt_vertex[i]->u;
		vertices[i].uv.y = meshData->m_opt_vertex[i]->v;
	}

	tcount = meshData->m_mesh_numfaces;

	count = 3 * tcount;
	std::vector<UINT> indices(count);
	for (UINT i = 0; i < tcount; ++i)
	{
		indices[i * 3 + 0] = meshData->m_opt_index[i].index[0];
		indices[i * 3 + 1] = meshData->m_opt_index[i].index[2];
		indices[i * 3 + 2] = meshData->m_opt_index[i].index[1];
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
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
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
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
