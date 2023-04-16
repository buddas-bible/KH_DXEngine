#pragma once

#include "../KH_Math/Vector2D.h"
#include "../KH_Math/Vector3D.h"
#include "../KH_Math/Vector4D.h"

struct PCVertex
{
	Vector3D pos;				// ���ؽ� ��ġ ����
	Vector4D color;
};

struct PNVertex
{
	Vector3D pos;				// ���ؽ� ��ġ ����
	Vector3D normal;
};

struct PTNVertex
{
	Vector3D pos;				// ���ؽ� ��ġ ����
	Vector2D uv;
	Vector3D normal;
};

struct PTNTVertex
{
	Vector3D pos;				// ���ؽ� ��ġ ����
	Vector2D uv;
	Vector3D normal;
	Vector3D tangent;
};