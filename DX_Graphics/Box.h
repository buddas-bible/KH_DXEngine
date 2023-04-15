#pragma once

#include "Object.h"


class Box : public Object
{
public:
	Box(
		Microsoft::WRL::ComPtr<ID3D11Device>& device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& randerState
	) : Object(device, context, randerState)
	{}
	virtual ~Box();

	virtual HRESULT Initialize() override;
	virtual HRESULT CreateBuffer() override;
	virtual HRESULT CreateLayout() override;
	virtual HRESULT CreateEffect() override;
	virtual HRESULT Finalize() override;
	virtual void Update(const Matrix4x4& view, const Matrix4x4& proj) override;
	virtual void Render() override;


private:
	Vector3D m_pos;
	Vector3D m_angle;
	Vector3D m_scale{1.f, 1.f, 1.f};

private:
	Matrix4x4 invT;
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
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_invMatrix;

	Microsoft::WRL::ComPtr<ID3DX11EffectShaderResourceVariable> m_shaderResource;	// 
	Microsoft::WRL::ComPtr<ID3DX11EffectSamplerVariable> m_sampler;		// ��� ���ø� ����. �⺻���� �̹� ����־ ������ ���� �����൵ �ؽ��Ĵ� ����

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;			/// ��� �Ǵ� ��

	Microsoft::WRL::ComPtr<ID3D11Resource>				m_texture;		/// �ʱ�ȭ ��ÿ��� �ʿ��ϰ� ������ ���� �ʿ�� ������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_textureView;	// �ؽ��� ��� ���� ģ��

	Microsoft::WRL::ComPtr<ID3DX11EffectVectorVariable> m_directionalLight;
	Microsoft::WRL::ComPtr<ID3DX11EffectVectorVariable> m_Light;
};
