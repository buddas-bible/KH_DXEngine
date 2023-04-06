#pragma once

#include "../KH_Math/Vector3D.h"
#include "../KH_Math/Vector4D.h"

class Camera
{
public:
	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetViewProjMatrix();

	void moveR();			// ī�޶� �������κ��� ���������� �̵�
	void moveU();			// ī�޶� �������κ��� ���� �̵�
	void moveL();			// ī�޶� �������κ��� ������ �̵�
	void Pitch();
	void Yaw();

private:
	Vector3D m_pos;			// ī�޶� ��ġ
	Vector3D m_look;		// ī�޶� z
	Vector3D m_up;			// ī�޶� y
	Vector3D m_right;		// ī�޶� x
};

