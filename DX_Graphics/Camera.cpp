#include "Camera.h"
#include "../KH_Math/Matrix3x3.h"
#include "../KH_Math/Matrix4x4.h"


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
