#pragma once

#include "Object.h"

namespace ASEParser
{
	class Mesh;
}

class Teapot : public Object
{
public:
	Teapot(
		Microsoft::WRL::ComPtr<ID3D11Device>& device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& randerState
	) : Object(device, context, randerState) {}
	virtual ~Teapot();

	virtual HRESULT Initialize() override;
	virtual HRESULT CreateBuffer() override;
	virtual HRESULT CreateLayout() override;
	virtual HRESULT CreateEffect() override;
	virtual HRESULT Finalize() override;
	virtual void Update(const Matrix4x4& view, const Matrix4x4& proj) override;
	virtual void Render() override;

	HRESULT LoadGeometry(ASEParser::Mesh* meshData);


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

};

