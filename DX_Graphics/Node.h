#pragma once

#include "DXDefine.h"

namespace ASEParser
{
	class Mesh;
}

class Node
{
public:
	Node(Microsoft::WRL::ComPtr<ID3D11Device>& device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& randerState
	);
	~Node() = default;

	Microsoft::WRL::ComPtr< ID3D11Device>& m_device;
	Microsoft::WRL::ComPtr< ID3D11DeviceContext>& m_context;
	Microsoft::WRL::ComPtr< ID3D11RasterizerState>& m_randerState;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;				// 나중에 없앨 예정

	Microsoft::WRL::ComPtr<ID3DX11Effect> m_effect;							// 이것도 없앨 예정
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique> m_tech;					// 이이이것도 없앨 예정
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_matrix;			// 이것도


private:
	UINT indexCount;

	std::wstring m_nodeName;
	Node* m_parent;
	ASEParser::Mesh* mesh;

	Vector3D m_pos;
	Vector3D m_angle;
	Vector3D m_scale;
	
	Matrix4x4 m_localTM;
	Matrix4x4 m_worldTM;
	Matrix4x4 m_viewTM;
	Matrix4x4 m_projTM;

public:
	/// Initialize
	HRESULT CreateGeometry();

	/// Get
	std::wstring& GetNodeName();
	Matrix4x4 GetWorldMatrix();

	/// Set
	void SetParent(Node* parent);
};

