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

	if (m_parent != nullptr)							// 부모가 있다면 부모의 월드에 자신의 로컬을 연산한 결과를 반환함.
	{													// 부모가 없다면 자신의 로컬을 반환함
		m_worldTM = m_localTM * m_parent->GetWorldMatrix();
		return m_worldTM;
	}

	return m_localTM;
}

void Node::SetParent(Node* parent)
{
	m_parent = parent;
}
