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

	void moveX(const float& d);			// 월드 X 방향 이동
	void moveY(const float& d);			// 월드 Y 방향 이동
	void moveZ(const float& d);			// 월드 Z 방향 이동

	void moveR(const float& d);			// 카메라 시점으로부터 오른쪽으로 이동
	void moveU(const float& d);			// 카메라 시점으로부터 위로 이동
	void moveL(const float& d);			// 카메라 시점으로부터 앞으로 이동

	void Pitch(const float& a);			// 카메라 위아래 회전
	void Yaw(const float& a);			// 카메라 좌우 회전

private:
	Vector3D m_pos;			// 카메라 위치
	Vector3D m_look;		// 카메라 z
	Vector3D m_up;			// 카메라 y
	Vector3D m_right;		// 카메라 x

	Matrix4x4 m_View;
	Matrix4x4 m_Proj;
};

