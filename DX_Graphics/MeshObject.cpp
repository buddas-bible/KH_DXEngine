#include "MeshObject.h"

#include <fstream>
#include "CParsingDataClass.h"
#include "CASEParser.h"
#include "TimeManager.h"

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
	HRESULT hr = S_OK;

	if (m_type != eGeomobject)
	{
		return S_FALSE;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(PTNVertex) * vertices.size();
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

HRESULT MeshObject::CreateEffect()
{
	HRESULT hr = S_OK;
	std::ifstream fin;
#if _WIN64

#if _DEBUG
	fin.open("../x64/debug/NonTextureBasic.cso", std::ios::binary);
#else
	fin.open("../x64/release/NonTextureBasic.cso", std::ios::binary);		// �̺κ� ��� �ٽ� �����غ���...
#endif

#else

#if _DEBUG
	fin.open("../WIN32/debug/NonTextureBasic.cso", std::ios::binary);
#else
	fin.open("../WIN32/release/NonTextureBasic.cso", std::ios::binary);
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
	m_invMatrix = m_effect->GetVariableByName("invTworldViewProj")->AsMatrix();
	m_light = m_effect->GetVariableByName("lightDirection");

	m_shaderResource = m_effect->GetVariableByName("g_Texture")->AsShaderResource();	// 
	istexture = m_effect->GetVariableByName("istexture");

	return hr;
}

HRESULT MeshObject::Finalize()
{
	return S_OK;

}

void MeshObject::Update(const Matrix4x4& view, const Matrix4x4& proj)
{
	TimeManager& time = TimeManager::GetInstance();

	if (GetAsyncKeyState(VK_BACK))
	{
		m_aniStackTime = 0;
	}

	if (playAnimation)
	{
		m_aniStackTime += time.GetfDeltaTime() * 4000.f;
	}
	else
	{
		m_animationTM;
		m_localTM = CreateMatrix(m_pos, m_axisAndAngle, m_scale);
	}

	m_worldTM = GetWorldMatrix();
	m_viewTM = view;
	m_projTM = proj;
}

void MeshObject::Render()
{
	if (vertices.empty())
	{
		return;
	}

	// �����Ͷ����� ������Ʈ
	m_context->RSSetState(m_randerState.Get());

	m_context->IASetInputLayout(m_inputLayout.Get());
	m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// IA�� ���ؽ� ���� ����
	UINT stride = sizeof(PTNVertex);
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

	DirectX::XMMATRIX World = ConvertToXMMATRIX(m_worldTM);
	DirectX::XMMATRIX d = World;
	d.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR det = DirectX::XMMatrixDeterminant(d);
	DirectX::XMMATRIX invTwold = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, World));

	// ���̴��� �ִ� ������ ��� ����
	m_matrix->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
	m_invMatrix->SetMatrix(reinterpret_cast<float*>(&invTwold));

	const float light[3] = { 0.5f, 0.5f, 0.f };
	m_light->SetRawValue(light, 0, sizeof(light));
	istexture->SetRawValue(&isTexture, 0, sizeof(isTexture));

	m_shaderResource->SetResource(m_textureView.Get());

	// ��ũ��
	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply(0, m_context.Get());

		m_context->DrawIndexed(count, 0, 0);
	}
}

/// <summary>
/// �������� �Լ��� ���� ���������� ������
/// 
/// �θ� �̸� �����ϴ� �뵵
/// </summary>
/// <param name="meshData"></param>
/// <returns></returns>
HRESULT MeshObject::LoadNodeData(ASEParser::Mesh* meshData)
{
	HRESULT hr = S_OK;

	// �޽� Ÿ�� ����
	m_type = meshData->m_type;

	// ��� �̸� ����
	nodeName.assign(meshData->m_nodename.begin(), meshData->m_nodename.end());

	// �θ� �̸� ����
	if (!meshData->m_isparentexist)
	{
		return S_FALSE;
	}
	parentName.assign(meshData->m_nodeparent.begin(), meshData->m_nodeparent.end());

	return hr;
}

/// <summary>
/// �θ� Ÿ�� �ö󰡸鼭 ������ ���س���
/// </summary>
/// <returns></returns>
Matrix4x4 MeshObject::GetWorldMatrix()
{
	Matrix4x4 w = Matrix4x4::ScaleMatrix(0.02f, 0.02f, 0.02f);
	// Matrix4x4 w = Matrix4x4::IdentityMatrix();

	if (parent != nullptr)
	{
		w = parent->GetWorldMatrix();
	}

	return m_localTM * w;
}

/// <summary>
/// �ִϸ��̼� ������ �ִ� POS, ROT�� ���� �����ε��ϴ�
/// �ϴ�... ������ ���� POS, ROT�� ������ ������ TM�� �����
/// �θ��� ���� TM�� �̿��ؼ� �ش� �������� ���� TM�� �̾Ƴ���.
/// ���� ������ ��� ������ ��εǴ� �κ�.
/// </summary>
/// <returns></returns>
void MeshObject::UpdateAnimaionTM()
{
	/// ��ŸŸ���� �װ� �����ӷ���Ʈ ���ϸ鼭
	/// ������ �ִϸ��̼� �����͸� ������
	/// ȸ���� �����ذ��鼭 �ؾ��ϰ�, ��ġ�� ó�� ��ġ�κ��� ��ȭ�Ѱɷ� ��.
	/// 
	/// � �����ӿ� �����Ͱ� �ִ��� �˰������� ������ ����.
	/// �׷��� ���ؼ� �ִϸ��̼� �����͸� �о�� �� �����ӷ���Ʈ�� ���� �����ϴ°� ���� ���δ�.
	/// �ش� Ű �����ӿ� �ƹ��͵� ���ٸ�? �׳� LocalTM.
	/// ���𰡰� ������ LocalTM�� ������ ����.
	/// 
	/// 

	using namespace DirectX;
	m_animationTM = Matrix4x4::IdentityMatrix();

	XMMATRIX animationTM = XMMatrixIdentity();
	XMMATRIX posTM, rotTM, sclTM;

	// sclTM = Matrix4x4::ScaleMatrix(m_scale.x, m_scale.y, m_scale.z);
	sclTM = DirectX::XMMatrixScalingFromVector(ConvertToXMVECTOR(m_scale));
	animationTM = animationTM * sclTM;

	if (nodeName == L"Biped-R_legup")
	{
		int a = 0;
	}

	// DirectX::XMMATRIX rrr = DirectX::XMMatrixRotationAxis({ XMVectorSet(m_axisAndAngle.x, m_axisAndAngle.z,m_axisAndAngle.z, 0.f) }, m_axisAndAngle.w);
	// DirectX::XMMATRIX rrrq = DirectX::XMMatrixRotationQuaternion(ConvertToXMVECTOR(m_quaternion));
	// Matrix4x4 rrar = ConvertToKHMatrix(rrr);
	auto& rotList = animationData.rotList;
	if (rotList.size() != 0)
	{
		size_t i = 0;
		for (; i < rotList.size(); i++)
		{
			if (rotList[i]->framerate > m_aniStackTime)
			{
				m_prevFrame = rotList[i - 1]->framerate;
				m_nextFrame = rotList[i]->framerate;
				break;
			}
			else if (rotList[rotList.size() - 1]->framerate < m_aniStackTime)
			{
				i = 1;
				m_aniStackTime = 0.f;
				break;
			}
			else
			{
				continue;
			}
		}
		XMVECTOR q1 = ConvertToXMVECTOR(rotList[i - 1]->quaternion);
		XMVECTOR q2 = ConvertToXMVECTOR(rotList[i]->quaternion);

		float f = (m_aniStackTime - m_prevFrame) / (m_nextFrame - m_prevFrame);			// ���� �������� ������ ��.

		XMVECTOR nowRot = XMQuaternionSlerp(q1, q2, f);

		rotTM = XMMatrixRotationQuaternion(nowRot);

		XMVECTOR axi{};
		float an;
		XMQuaternionToAxisAngle(&axi, &an, nowRot);
		Matrix4x4 r = RotationAxisAngle(ConvertToKHVector4D(axi), an);
	}
	else
	{
		rotTM = DirectX::XMMatrixRotationQuaternion(ConvertToXMVECTOR(m_quaternion));
	}

	// rotTM = ConvertToKHMatrix(DirectX::XMMatrixRotationQuaternion(ConvertToXMVECTOR(m_quaternion)));
	/// �� ���ʹϾ����� ȸ�� ����� ���� ����, �ఢ���� ���鶧�� ���� �ٸ�����? ��°����?
	// rotTM = AxisAndAngleRotation({ m_axisAndAngle.x, m_axisAndAngle.y , m_axisAndAngle.z }, m_axisAndAngle.w);
	animationTM = animationTM * rotTM;

	// ����Ʈ�� ũ�Ⱑ 0�� �ƴϸ� �����ӷ���Ʈ ���ؼ�
	// ���� ���̿� �ִ��� ���� ������ �� 
	// 0�̶�� �׳� �⺻��
	auto& posList = animationData.posList;
	if (posList.size() != 0)
	{
		size_t i = 0;
		for (; i < posList.size(); i++)
		{
			if (posList[i]->framerate > m_aniStackTime)
			{
				m_prevFrame = posList[i - 1]->framerate;
				m_nextFrame = posList[i]->framerate;
				break;
			}
			else if (posList[animationData.posList.size() - 1]->framerate < m_aniStackTime)
			{
				i = 1;
				m_aniStackTime = 0.f;
				break;
			}
			else
			{
				continue;
			}
		}
		XMVECTOR v1 = ConvertToXMVECTOR({ posList[i - 1]->pos , 1.f });
		XMVECTOR v2 = ConvertToXMVECTOR({ posList[i]->pos , 1.f });
		
		float f = (m_aniStackTime - m_prevFrame) / (m_nextFrame - m_prevFrame);			// ���� �������� ������ ��.
		XMVECTOR nowPos = XMVectorLerp(v1, v2, f);

		posTM = XMMatrixTranslationFromVector(nowPos);
	}
	else
	{
		posTM = XMMatrixTranslationFromVector(ConvertToXMVECTOR({ m_pos, 1.f}));
	}

	animationTM = animationTM * posTM;

	if (nodeName == L"Text02")
	{
		int i = 0;
	}

	m_localTM = ConvertToKHMatrix(animationTM);

	/// 1-1. Pos? ù ���������κ��� ��ȭ
	/// 1-2. Rot? ���� ���������κ����� ��ȭ
	/// 2. ����ִ� Framerate ó��
}

/// <summary>
/// �θ� ������ ������� ���ؼ� �ڽ��� ������ �̾Ƴ�
/// </summary>
void MeshObject::InitializeLocalTM()
{
	using namespace DirectX;
	Matrix4x4 w = Matrix4x4::IdentityMatrix();

	if (parent != nullptr)
	{
		w = InverseMatrix(parent->m_nodeTM);
	}

	// �θ� ��� ����� ������� ���� ���� ����� ���Ѵ�.
	m_localTM = m_nodeTM * w;

	XMVECTOR scl{};
	XMVECTOR rot{};
	XMVECTOR trs{};
	XMMATRIX local = ConvertToXMMATRIX(m_localTM);
	XMMatrixDecompose(&scl, &rot, &trs, local);

	m_pos.x = XMVectorGetX(trs);
	m_pos.y = XMVectorGetY(trs);
	m_pos.z = XMVectorGetZ(trs);

	m_quaternion = ConvertToKHVector4D(rot);

	if (XMQuaternionIsInfinite(rot))
	{
		m_axisAndAngle = Vector4D{ 1.f, 0.f, 0.f, 0.f };
	}
	else
	{
		m_axisAndAngle = QuaternionToAxisAngle(ConvertToKHVector4D(rot));
	}

	/// negative scale Ȯ��?
	// Vector3D rx = { m_nodeTM.e[0][0] , m_nodeTM.e[0][1] , m_nodeTM.e[0][2] };
	// Vector3D ry = { m_nodeTM.e[1][0] , m_nodeTM.e[1][1] , m_nodeTM.e[1][2] };
	// Vector3D rz = { m_nodeTM.e[2][0] , m_nodeTM.e[2][1] , m_nodeTM.e[2][2] };
	// 
	// static std::vector<std::wstring> a;
	// Vector3D zCrossX = rz.Cross(rx);
	// float theta = zCrossX.Dot(ry);
	// if (theta < 0)
	// {
	// 	for (size_t i = 0; i < 3; i++)
	// 	{
	// 		if (scl.m128_f32[i] < 0.f)
	// 		{
	// 			scl.m128_f32[i] *= -1.f;
	// 			break;
	// 		}
	// 	}
	// }

	m_scale.x = XMVectorGetX(scl);
	m_scale.y = XMVectorGetY(scl);
	m_scale.z = XMVectorGetZ(scl);

// 	if (m_scale.x < 0.f)
// 	{
// 		// m_scale.x *= -1.f;
// 	}
// 	if (m_scale.z < 0.f)
// 	{
// 		m_scale.z *= -1.f;
// 	}
}

HRESULT MeshObject::LoadAnimation(CASEParser* aniList)
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT MeshObject::LoadGeometry(ASEParser::Mesh* meshData)
{
	HRESULT hr = S_OK;

	m_nodeTM.e[0][0] = meshData->m_tm_row0.x;
	m_nodeTM.e[0][1] = meshData->m_tm_row0.y;
	m_nodeTM.e[0][2] = meshData->m_tm_row0.z;

	m_nodeTM.e[1][0] = meshData->m_tm_row1.x;
	m_nodeTM.e[1][1] = meshData->m_tm_row1.y;
	m_nodeTM.e[1][2] = meshData->m_tm_row1.z;

	m_nodeTM.e[2][0] = meshData->m_tm_row2.x;
	m_nodeTM.e[2][1] = meshData->m_tm_row2.y;
	m_nodeTM.e[2][2] = meshData->m_tm_row2.z;

	m_nodeTM.e[3][0] = meshData->m_tm_row3.x;
	m_nodeTM.e[3][1] = meshData->m_tm_row3.y;
	m_nodeTM.e[3][2] = meshData->m_tm_row3.z;

	if (meshData->m_rawVertex.empty())
	{
		return S_FALSE;
	}

	isHelperObj = true;

	Matrix4x4 invTM = InverseMatrix(m_nodeTM);					// ���� ����� ���ؽ��� ���÷� ������������ ��
	Matrix4x4 invTransTM = InverseTransposeMatrix(m_nodeTM);	// ���� ������� ��ġ

	UINT vcount = 0;
	UINT tcount = 0;

	vcount = meshData->m_rawVertex.size();
	vertices.resize(vcount);

	for (int i = 0; i < vcount; i++)
	{
		vertices[i].pos.x = meshData->m_rawVertex[i]->m_pos.x;
		vertices[i].pos.y = meshData->m_rawVertex[i]->m_pos.y;
		vertices[i].pos.z = meshData->m_rawVertex[i]->m_pos.z;

		// ���� �������� �Ǿ��ִ� ���ؽ��� ������ ������� �����༭ ���÷� �������´�.
		vertices[i].pos = Vector4D(vertices[i].pos, 1.f) * invTM; // �𵨸� ��ü�� �ʹ� Ŀ�� �ٿ���.

		vertices[i].uv.x = meshData->m_rawVertex[i]->u;
		vertices[i].uv.y = meshData->m_rawVertex[i]->v;

		vertices[i].normal.x = meshData->m_rawVertex[i]->m_normal.x;
		vertices[i].normal.y = meshData->m_rawVertex[i]->m_normal.y;
		vertices[i].normal.z = meshData->m_rawVertex[i]->m_normal.z;

		// �븻�� �и� Ʈ�������� �޽��� �������� ���������� ������ �����Ǳ� ������
		// ������ ����� ���ٵ�...
		// ������ ������� ��ġ�� �޾Ƽ� ��ȯ�� �븻?
		// �׷� �븻
		vertices[i].normal = Vector4D(vertices[i].normal, 1.f);
	}

	count = meshData->m_rawIndex.size();

	int inv = count / 3;
	indices.resize(count);
	for (UINT i = 0; i < inv; ++i)
	{
		indices[i * 3 + 0] = meshData->m_rawIndex[i * 3 + 0];
		indices[i * 3 + 1] = meshData->m_rawIndex[i * 3 + 1];
		indices[i * 3 + 2] = meshData->m_rawIndex[i * 3 + 2];
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

void MeshObject::SetAnimation()
{
	if (playAnimation)
	{
		playAnimation = false;
		return;
	}

	playAnimation = true;
}

Vector3D MeshObject::GetLocalPosition()
{
	return Vector3D();
	// return Matrix4x4(
	// 	1.f,				0.f,				0.f,			0.f,
	// 	0.f,				1.f,				0.f,			0.f,
	// 	0.f,				0.f,				1.f,			0.f,
	// 	m_localTM.e[3][0], m_localTM.e[3][1], m_localTM.e[3][2], 1.f);
}

Vector3D MeshObject::GetLocalRotate()
{
	return  Vector3D();
	// DirectX::XMMatrixDecompose();
}

HRESULT MeshObject::LoadTexture(const wstring& path)
{
	HRESULT hr = S_OK;

	if (path.empty())
	{
		isTexture = false;
		return S_FALSE;
	}

	ComPtr<ID3D11Resource> texture;
	ComPtr<ID3D11ShaderResourceView> textureView;				// ../ ���� ��δ� �ٽ� �����غ���.. �������� ���� �� ���� ������ ����...
	hr = DirectX::CreateDDSTextureFromFile(m_device.Get(), path.c_str(), texture.GetAddressOf(), textureView.GetAddressOf());
	if (FAILED(hr))
	{
		isTexture = false;
		return hr;
	}
	texture.As(&m_texture);
	textureView.As(&m_textureView);

	return hr;
}
