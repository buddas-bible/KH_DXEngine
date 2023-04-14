#pragma once

#include "Object.h"

class Grid : public Object
{
public:
	Grid(
		Microsoft::WRL::ComPtr<ID3D11Device>& device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& randerState
	) : Object(device, context, randerState)
	{}
	virtual ~Grid();

	virtual HRESULT Initialize() override;
	virtual HRESULT CreateBuffer() override;
	virtual HRESULT CreateLayout() override;
	virtual HRESULT CreateEffect() override;
	virtual HRESULT Finalize() override;
	virtual void Update(const Matrix4x4& view, const Matrix4x4& proj) override;
	virtual void Render() override;

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

