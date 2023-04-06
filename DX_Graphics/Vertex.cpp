#include "Vertex.h"

#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 pos;				// ���ؽ� ��ġ ����
	DirectX::XMFLOAT4 color;			// ���ؽ� �÷� ����
};

struct TextureVertex
{
	DirectX::XMFLOAT3 pos;				// ���ؽ� ��ġ ����
	DirectX::XMFLOAT3 normal;			// ���ؽ� �븻 ����
	DirectX::XMFLOAT2 uv;				// ���ؽ� �ؽ��� UV ��ǥ
	DirectX::XMFLOAT4 color;			// ���ؽ� �÷� ����
};

struct ConstantBuffer
{
	DirectX::XMMATRIX world;			// ���� ��Ʈ����
	DirectX::XMMATRIX view;			// �� ��Ʈ����
	DirectX::XMMATRIX projection;		// ���� ��Ʈ����
};