#pragma once

#include "Object.h"
#include <string>
#include <vector>

namespace ASEParser
{
	class Mesh;
}

class MeshObject : public Object
{
public:
	MeshObject(
		Microsoft::WRL::ComPtr<ID3D11Device>& device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& randerState
	) : Object(device, context, randerState) {}
	virtual ~MeshObject();

	virtual HRESULT Initialize() override;
	virtual HRESULT CreateBuffer() override;
	virtual HRESULT CreateLayout() override;
	virtual HRESULT CreateEffect() override;
	virtual HRESULT Finalize() override;
	virtual void Update(const Matrix4x4& view, const Matrix4x4& proj) override;
	virtual void Render() override;

	HRESULT LoadNodeData(ASEParser::Mesh* meshData);
	HRESULT LoadAnimation(ASEParser::Mesh* meshData);
	HRESULT LoadGeometry(ASEParser::Mesh* meshData);
	HRESULT LoadTexture(const std::wstring& path);

	void SetScalse(Vector3D scale);
	void SetPosition(Vector3D position);

private:
	std::wstring nodeName;

	Vector3D m_pos{};
	Vector3D m_angle{ 0.f, 0.f, 0.f };
	Vector3D m_scale{ 0.01f, 0.01f, 0.01f };
	bool isTexture{true};

private:
	// UINT count;
	// Matrix4x4 worldTM;
	// 
	// Microsoft::WRL::ComPtr< ID3D11Device> m_device;
	// Microsoft::WRL::ComPtr< ID3D11DeviceContext> m_context;
	// Microsoft::WRL::ComPtr< ID3D11RasterizerState> m_randerState;
	// 
	// Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	// Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	// 
	// Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 
	// Microsoft::WRL::ComPtr<ID3DX11Effect> m_effect;
	// Microsoft::WRL::ComPtr<ID3DX11EffectTechnique> m_tech;
	// Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_matrix;

	/*
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_invMatrix;
	*/
	Microsoft::WRL::ComPtr<ID3DX11EffectShaderResourceVariable> m_shaderResource;	// 

	Microsoft::WRL::ComPtr<ID3D11Resource>				m_texture;		/// 초기화 당시에만 필요하고 가지고 있을 필요는 없을듯
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_textureView;	// 텍스쳐 들고 있을 친구

	Microsoft::WRL::ComPtr<ID3DX11EffectVariable> m_light;

	Microsoft::WRL::ComPtr<ID3DX11EffectVariable> istexture;
};

