#pragma once

#include "../KH_Math/Vector3D.h"
#include "../KH_Math/Vector4D.h"


class Camera
{
public:
	Matrix4x4 SetViewMatrix();
	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetProjMatrix();
	Matrix4x4 GetViewProjMatrix();

	void moveX(const float& d);			// ���� X ���� �̵�
	void moveY(const float& d);			// ���� Y ���� �̵�
	void moveZ(const float& d);			// ���� Z ���� �̵�

	void moveR(const float& d);			// ī�޶� �������κ��� ���������� �̵�
	void moveU(const float& d);			// ī�޶� �������κ��� ���� �̵�
	void moveL(const float& d);			// ī�޶� �������κ��� ������ �̵�

	void Pitch(const float& a);			// ī�޶� ���Ʒ� ȸ��
	void Yaw(const float& a);			// ī�޶� �¿� ȸ��

private:
	Vector3D m_pos;			// ī�޶� ��ġ
	Vector3D m_look;		// ī�޶� z
	Vector3D m_up;			// ī�޶� y
	Vector3D m_right;		// ī�޶� x

	Matrix4x4 m_View;
	Matrix4x4 m_Proj;
};

