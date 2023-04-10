#include "Camera.h"
#include "../KH_Math/Matrix3x3.h"
#include "../KH_Math/Matrix4x4.h"


void Camera::Update()
{
	// 카메라의 기저들과
	// 카메라 위치를 가지고 View 행렬을 업데이트 함.
	// 마지막 행에는 기저에 포지션을 내적하고 음수를 취한 것을 넣어야함


	m_View.e[0][0];
	m_View.e[0][1];
	m_View.e[0][2];
	m_View.e[0][3];

	m_View.e[1][0];
	m_View.e[1][1];
	m_View.e[1][2];
	m_View.e[1][3];

	m_View.e[2][0];
	m_View.e[2][1];
	m_View.e[2][2];
	m_View.e[2][3];

	m_View.e[3][0];
	m_View.e[3][1];
	m_View.e[3][2];
	m_View.e[3][3] = 0.f;
}

Matrix4x4 Camera::GetViewMatrix()
{
	return m_View;
}

Matrix4x4 Camera::GetProjMatrix()
{
	return m_Proj;
}

Matrix4x4 Camera::GetViewProjMatrix()
{
	return m_View * m_Proj;
}

void Camera::moveX(const float& d)
{
	this->m_pos.x += d;
}

void Camera::moveY(const float& d)
{
	this->m_pos.y += d;
}

void Camera::moveZ(const float& d)
{
	this->m_pos.z += d;
}

void Camera::moveR(const float& d)
{
	m_right = m_right.Normalize();
	m_pos += m_right * d;
}

void Camera::moveU(const float& d)
{
	m_up = m_up.Normalize();
	m_pos += m_up * d;
}

void Camera::moveL(const float& d)
{
	m_right = m_look.Normalize();
	m_pos += m_look * d;
}

void Camera::Pitch(const float& a)
{
	Matrix3x3 rotate = AxisAndAngleRotation(m_right, a);

	m_look = m_look.Normalize() * rotate;
	m_up = m_up.Normalize() * rotate;
}

void Camera::Yaw(const float& a)
{
	Matrix3x3 rotate = AxisAndAngleRotation(m_up, a);

	m_look = m_look.Normalize() * rotate;
	m_right = m_right.Normalize() * rotate;
}
