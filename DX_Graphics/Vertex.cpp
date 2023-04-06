#include "Vertex.h"

#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 pos;				// 버텍스 위치 정보
	DirectX::XMFLOAT4 color;			// 버텍스 컬러 정보
};

struct TextureVertex
{
	DirectX::XMFLOAT3 pos;				// 버텍스 위치 정보
	DirectX::XMFLOAT3 normal;			// 버텍스 노말 정보
	DirectX::XMFLOAT2 uv;				// 버텍스 텍스쳐 UV 좌표
	DirectX::XMFLOAT4 color;			// 버텍스 컬러 정보
};

struct ConstantBuffer
{
	DirectX::XMMATRIX world;			// 월드 매트릭스
	DirectX::XMMATRIX view;			// 뷰 매트릭스
	DirectX::XMMATRIX projection;		// 투영 매트릭스
};