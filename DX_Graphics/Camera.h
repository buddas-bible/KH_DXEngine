#pragma once

#include "../KH_Math/Vector3D.h"
#include "../KH_Math/Vector4D.h"
#include "../KH_Math/Matrix4x4.h"

class Camera
{
public:
	void Update();
	Matrix4x4 UpdateViewMatrix();
	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetProjMatrix();
	Matrix4x4 GetViewProjMatrix();

	void CameraLookAtLH(const Vector3D& eyePos, const Vector3D& focus, const Vector3D& worldUp);
	void XMLookAtLH(const Vector3D& eyePos, const Vector3D& focus, const Vector3D& worldUp);
	void CameraLookToLH(const Vector3D& eyePos, const Vector3D& direction, const Vector3D& worldUp);
	void XMLookToLH(const Vector3D& eyePos, const Vector3D& direction, const Vector3D& worldUp);
	void CameraPerspectiveFovLH(float angleY, float ratio, float near, float far);
	void XMPerspectiveFovLH(float angleY, float ratio, float near, float far);
	void CameraOrthographicLH(float width, float height, float near, float far);
	void XMOrthographicLH(float width, float height, float near, float far);

	void moveX(const float& d);			// ���� X ���� �̵�
	void moveY(const float& d);			// ���� Y ���� �̵�
	void moveZ(const float& d);			// ���� Z ���� �̵�

	void moveR(const float& d);			// ī�޶� �������κ��� ���������� �̵�
	void moveU(const float& d);			// ī�޶� �������κ��� ���� �̵�
	void moveL(const float& d);			// ī�޶� �������κ��� ������ �̵�

	void Pitch(const float& a);			// ī�޶� ���Ʒ� ȸ��
	void Yaw(const float& a);			// ī�޶� �¿� ȸ��

	void SetViewportHW(float height, float width);
	void SetPerspectiveView();
	void SetOrthographicView();

private:
	Vector3D worldUp{0.f, 1.f, 0.f};

	Vector3D m_pos{};			// ī�޶� ��ġ
	Vector3D m_look{};		// ī�޶� z
	Vector3D m_up{};			// ī�޶� y
	Vector3D m_right{};		// ī�޶� x

	Matrix4x4 m_View{};
	Matrix4x4 m_Proj{};

	float m_angle;
	float m_ratio;
	float m_height;
	float m_width;
	float m_near;
	float m_far;
};

