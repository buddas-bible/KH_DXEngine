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

	void moveX(const float& d);			// 월드 X 방향 이동
	void moveY(const float& d);			// 월드 Y 방향 이동
	void moveZ(const float& d);			// 월드 Z 방향 이동

	void moveR(const float& d);			// 카메라 시점으로부터 오른쪽으로 이동
	void moveU(const float& d);			// 카메라 시점으로부터 위로 이동
	void moveL(const float& d);			// 카메라 시점으로부터 앞으로 이동

	void Pitch(const float& a);			// 카메라 위아래 회전
	void Yaw(const float& a);			// 카메라 좌우 회전

	void SetViewportHW(float height, float width);
	void SetPerspectiveView();
	void SetOrthographicView();

private:
	Vector3D worldUp{0.f, 1.f, 0.f};

	Vector3D m_pos{};			// 카메라 위치
	Vector3D m_look{};		// 카메라 z
	Vector3D m_up{};			// 카메라 y
	Vector3D m_right{};		// 카메라 x

	Matrix4x4 m_View{};
	Matrix4x4 m_Proj{};

	float m_angle;
	float m_ratio;
	float m_height;
	float m_width;
	float m_near;
	float m_far;
};

