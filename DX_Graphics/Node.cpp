#include "Node.h"

using namespace Microsoft;
using namespace Microsoft::WRL;

Node::Node(
	ComPtr<ID3D11Device>& device,
	ComPtr<ID3D11DeviceContext>& context,
	ComPtr<ID3D11RasterizerState>& randerState) :
	m_device(device), m_context(context), m_randerState(randerState),
	indexCount(0), m_worldTM(), m_viewTM(), m_projTM(),
	m_vertexBuffer(), m_indexBuffer(), m_inputLayout(),
	m_effect(), m_tech(), m_matrix(), m_parent(), mesh()
{

}

HRESULT Node::CreateGeometry()
{
	return S_FALSE;
}

std::wstring& Node::GetNodeName()
{
	return m_nodeName;
}

Matrix4x4 Node::GetWorldMatrix()
{
	m_localTM = CreateMatrix(m_pos, m_angle, m_scale);

	if (m_parent != nullptr)							// �θ� �ִٸ� �θ��� ���忡 �ڽ��� ������ ������ ����� ��ȯ��.
	{													// �θ� ���ٸ� �ڽ��� ������ ��ȯ��
		m_worldTM = m_localTM * m_parent->GetWorldMatrix();
		return m_worldTM;
	}

	return m_localTM;
}

void Node::SetParent(Node* parent)
{
	m_parent = parent;
}
