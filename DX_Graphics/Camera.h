#pragma once

#include "../KH_Math/Vector3D.h"
#include "../KH_Math/Vector4D.h"

class Camera
{
public:
	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetViewProjMatrix();

	void moveR();			// 카메라 시점으로부터 오른쪽으로 이동
	void moveU();			// 카메라 시점으로부터 위로 이동
	void moveL();			// 카메라 시점으로부터 앞으로 이동
	void Pitch();
	void Yaw();

private:
	Vector3D m_pos;			// 카메라 위치
	Vector3D m_look;		// 카메라 z
	Vector3D m_up;			// 카메라 y
	Vector3D m_right;		// 카메라 x
};

