#pragma once

#include "Object.h"
#include <string>

namespace ASEParser
{
	class Mesh;
}

class MeshObject : public Object
{
public:
	enum class eParsingMode
	{
		ASE,
		BLENDER,

	};

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

	HRESULT LoadGeometry(ASEParser::Mesh* meshData);
	HRESULT LoadASE(ASEParser::Mesh*& meshData);
	HRESULT LoadBLENDER(ASEParser::Mesh*& meshData);

	void SetScalse(Vector3D scale);
	void SetPosition(Vector3D position);

	void SetMode(eParsingMode _mode);
	HRESULT SetTexture(const std::wstring& path);

private:
	Vector3D m_pos{ -3.f, 0.f, 3.f };
	Vector3D m_angle{ 0.f, 0.f, 0.f };
	Vector3D m_scale{ 0.5f, 0.5f, 0.5f };
	bool isTexture{false};
	eParsingMode mode = eParsingMode::ASE;

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
	Microsoft::WRL::ComPtr<ID3DX11EffectSamplerVariable> m_sampler;		// ��� ���ø� ����. �⺻���� �̹� ����־ ������ ���� �����൵ �ؽ��Ĵ� ����

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;			/// ��� �Ǵ� ��

	Microsoft::WRL::ComPtr<ID3D11Resource>				m_texture;		/// �ʱ�ȭ ��ÿ��� �ʿ��ϰ� ������ ���� �ʿ�� ������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_textureView;	// �ؽ��� ��� ���� ģ��

	Microsoft::WRL::ComPtr<ID3DX11EffectVariable> m_light;
};

