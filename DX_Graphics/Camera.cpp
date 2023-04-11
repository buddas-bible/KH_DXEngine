#include "Camera.h"
#include "../KH_Math/Matrix3x3.h"


/// <summary>
/// 매 프레임 뷰 행렬을 업데이트 함
/// </summary>
void Camera::Update()
{
	// 카메라의 기저들과
	// 카메라 위치를 가지고 View 행렬을 업데이트 함.
	// 마지막 행에는 기저에 포지션을 내적하고 음수를 취한 것을 넣어야함
	
	// 월드 up을 look을 외적 = right
	// look에 right를 외적 = up
	m_look = m_look.Normalize();
	m_right = (worldUp.Cross(m_look)).Normalize();
	m_up = (m_look.Cross(m_right)).Normalize();

	m_View.e[0][0] = m_right.x;
	m_View.e[0][1] = m_up.x;
	m_View.e[0][2] = m_look.x;
	m_View.e[0][3] = 0.f;

	m_View.e[1][0] = m_right.y;
	m_View.e[1][1] = m_up.y;
	m_View.e[1][2] = m_look.y;
	m_View.e[1][3] = 0.f;

	m_View.e[2][0] = m_right.z;
	m_View.e[2][1] = m_up.z;
	m_View.e[2][2] = m_look.z;
	m_View.e[2][3] = 0.f;

	m_View.e[3][0] = -(m_pos.Dot(m_right));
	m_View.e[3][1] = -(m_pos.Dot(m_up));
	m_View.e[3][2] = -(m_pos.Dot(m_look));
	m_View.e[3][3] = 1.f;
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

/// <summary>
/// 왼손 좌표계 기준으로 카메라 기저를 만듬
/// </summary>
void Camera::CameraLookAtLH(const Vector3D& eyePos, const Vector3D& focus, const Vector3D& worldUp)
{
	Vector3D look = (focus - eyePos).Normalize();
	Vector3D right = worldUp.Normalize().Cross(look);
	Vector3D up = look.Cross(right);

	m_look = look;
	m_right = right;
	m_up = up;
	m_pos = eyePos;

	m_View.e[0][0] = m_right.x;
	m_View.e[0][1] = m_up.x;
	m_View.e[0][2] = m_look.x;
	m_View.e[0][3] = 0.f;

	m_View.e[1][0] = m_right.y;
	m_View.e[1][1] = m_up.y;
	m_View.e[1][2] = m_look.y;
	m_View.e[1][3] = 0.f;

	m_View.e[2][0] = m_right.z;
	m_View.e[2][1] = m_up.z;
	m_View.e[2][2] = m_look.z;
	m_View.e[2][3] = 0.f;

	m_View.e[3][0] = -(m_pos.Dot(m_right));
	m_View.e[3][1] = -(m_pos.Dot(m_up));
	m_View.e[3][2] = -(m_pos.Dot(m_look));
	m_View.e[3][3] = 1.f;
}

/// <summary>
/// 왼손 좌표계 기준으로 카메라 기저를 만듬
/// </summary>
void Camera::CameraLookToLH(const Vector3D& eyePos, const Vector3D& direction, const Vector3D& worldUp)
{
	Vector3D look = direction.Normalize();
	Vector3D right = worldUp.Normalize().Cross(look);
	Vector3D up = look.Cross(right);

	m_look = look;
	m_right = right;
	m_up = up;
	m_pos = eyePos;

	m_View.e[0][0] = m_right.x;
	m_View.e[0][1] = m_up.x;
	m_View.e[0][2] = m_look.x;
	m_View.e[0][3] = 0.f;

	m_View.e[1][0] = m_right.y;
	m_View.e[1][1] = m_up.y;
	m_View.e[1][2] = m_look.y;
	m_View.e[1][3] = 0.f;

	m_View.e[2][0] = m_right.z;
	m_View.e[2][1] = m_up.z;
	m_View.e[2][2] = m_look.z;
	m_View.e[2][3] = 0.f;

	m_View.e[3][0] = -(m_pos.Dot(m_right));
	m_View.e[3][1] = -(m_pos.Dot(m_up));
	m_View.e[3][2] = -(m_pos.Dot(m_look));
	m_View.e[3][3] = 1.f;
}

/// <summary>
/// 왼손 좌표계 기준으로 투영 행렬을 만듬
/// </summary>
void Camera::CameraPerspectiveFovLH(float angleY, float ratio, float near, float far)
{
	
}

/// <summary>
/// 왼손 좌표계 기준으로 투영 행렬을 만듬
/// </summary>
void Camera::CameraOrthographicLH(float width, float height, float near, float far)
{

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
	float angle = (3.14159265f * a) / 180;

	Matrix3x3 rotate = AxisAndAngleRotation(m_right, a);

	m_look = m_look.Normalize() * rotate;
	// m_up = m_up.Normalize() * rotate;
}

void Camera::Yaw(const float& a)
{
	float angle = (3.14159265f * a) / 180;

	Matrix3x3 rotate = AxisAndAngleRotation(m_up, a);

	m_look = m_look.Normalize() * rotate;
	// m_right = m_right.Normalize() * rotate;
}
