#pragma once

#include "DXDefine.h"
#include "Vertex.h"

class Object
{
public:
	Object(
		const Microsoft::WRL::ComPtr<ID3D11Device> device,
		const Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
		const Microsoft::WRL::ComPtr<ID3D11RasterizerState> randerState
	) : m_device(device), m_context(context), m_randerState(randerState),
		count(0), m_worldTM(), m_viewTM(), m_projTM(),
		m_vertexBuffer(), m_indexBuffer(), m_inputLayout(),
		m_effect(), m_tech(), m_matrix()
	{}
	virtual ~Object() 
	{};

	virtual HRESULT Initialize() abstract;
	virtual HRESULT CreateBuffer() abstract;
	virtual HRESULT CreateLayout() abstract;
	virtual HRESULT CreateEffect() abstract;
	virtual HRESULT Finalize() abstract;
	virtual void Update(const Matrix4x4& view, const Matrix4x4& proj) abstract;
	virtual void Render() abstract;

protected:
	UINT count;

	Matrix4x4 m_worldTM;
	Matrix4x4 m_viewTM;
	Matrix4x4 m_projTM;

	Microsoft::WRL::ComPtr< ID3D11Device> m_device;
	Microsoft::WRL::ComPtr< ID3D11DeviceContext> m_context;
	Microsoft::WRL::ComPtr< ID3D11RasterizerState> m_randerState;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	Microsoft::WRL::ComPtr<ID3DX11Effect> m_effect;
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique> m_tech;
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_matrix;
};