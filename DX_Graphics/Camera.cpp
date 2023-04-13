#include "Camera.h"
#include "../KH_Math/Matrix3x3.h"

#include <utility>;


/// <summary>
/// �� ������ �� ����� ������Ʈ ��
/// </summary>
void Camera::Update()
{
	// ī�޶��� �������
	// ī�޶� ��ġ�� ������ View ����� ������Ʈ ��.
	// ������ �࿡�� ������ �������� �����ϰ� ������ ���� ���� �־����
	
	// ���� up�� look�� ���� = right
	// look�� right�� ���� = up
	m_View = UpdateViewMatrix();

	if (isPerspective)
	{
		m_Proj = m_persProj;
	}
	else
	{
		m_Proj = m_orthProj;
	}
}

Matrix4x4 Camera::UpdateViewMatrix()
{
	m_look = m_look.Normalize();
	m_right = (worldUp.Cross(m_look)).Normalize();
	m_up = (m_look.Cross(m_right)).Normalize();

	Matrix4x4 view
	(
		m_right.x, m_up.x, m_look.x, 0.f,
		m_right.y, m_up.y, m_look.y, 0.f,
		m_right.z, m_up.z, m_look.z, 0.f,
		-(m_pos.Dot(m_right)), -(m_pos.Dot(m_up)), -(m_pos.Dot(m_look)), 1.f
	);

	return view;
}

Matrix4x4 Camera::GetViewMatrix() const
{
	return m_View;
}

Matrix4x4 Camera::GetProjMatrix() const
{
	return m_Proj;
}

Matrix4x4 Camera::GetViewProjMatrix() const
{
	return m_View * m_Proj;
}

/// <summary>
/// �޼� ��ǥ�� �������� ī�޶� ������ ����
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

	m_View = UpdateViewMatrix();
}

void Camera::XMLookAtLH(const Vector3D& eyePos, const Vector3D& focus, const Vector3D& worldUp)
{

}

/// <summary>
/// �޼� ��ǥ�� �������� ī�޶� ������ ����
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

	m_View = UpdateViewMatrix();
}

void Camera::XMLookToLH(const Vector3D& eyePos, const Vector3D& direction, const Vector3D& worldUp)
{

}

/// <summary>
/// �޼� ��ǥ�� �������� ���� ����� ����
/// </summary>
void Camera::CameraPerspectiveFovLH(float angleY, float ratio, float near, float far)
{
	float nearH = 2.f * near * tanf(angleY * 0.5f);		// �Ͼ������ �Ÿ� * ź��Ʈ �ݽþ߰� (-1 1�� �ٲ��ֱ� ���� 2����)
	float nearW = nearH * ratio;						// ȭ�� ������ ���̸� ���ϴϱ� �ʺ� ����
	float scaleX = 2.f * near / nearW;					// 2 * n / w
	float scaleY = 2.f * near / nearH;					// 2 * n / h
	float scaleZ = far / (far - near);					// f / f - n
	float translate = -near * scaleZ;		// - nf / f - n

	m_angle = angleY;
	m_ratio = ratio;
	m_near = near;
	m_far = far;

	m_persProj = Matrix4x4
	(
		scaleX, 0.f, 0.f, 0.f,
		0.f, scaleY, 0.f, 0.f,
		0.f, 0.f, scaleZ, 1.f,
		0.f, 0.f, translate, 0.f
	);
}

void Camera::XMPerspectiveFovLH(float angleY, float ratio, float near, float far)
{
	// DirectX::XMMatrixPerspectiveFovLH(angleY, ratio, near, far);
}

/// <summary>
/// �޼� ��ǥ�� �������� ���� ����� ����
/// </summary>
void Camera::CameraOrthographicLH(float width, float height, float near, float far)
{
	float scaleX = 2.f / width;
	float scaleY = 2.f / height;
	float scaleZ = 1.f / (far - near);
	float translate = -near * scaleZ;

	m_height = height;
	m_width = width;
	m_near = near;
	m_far = far;

	m_orthProj = Matrix4x4
	(
		scaleX, 0.f, 0.f, 0.f,
		0.f, scaleY, 0.f, 0.f,
		0.f, 0.f, scaleZ, 0.f,
		0.f, 0.f, translate, 1.f
	);
}

void Camera::XMOrthographicLH(float width, float height, float near, float far)
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
	float angle = (3.14159265358979323846f * a) / 180;

	Matrix3x3 rotate = AxisAndAngleRotation(m_right, a);

	m_look = m_look.Normalize() * rotate;
}

void Camera::Yaw(const float& a)
{
	float angle = (3.14159265358979323846f * a) / 180;

	Matrix3x3 rotate = AxisAndAngleRotation(m_up, a);

	m_look = m_look.Normalize() * rotate;
}

void Camera::SetPerspectiveView()
{
	isPerspective = true;
	/*
	float nearH = 2.f * m_near * tanf(m_angle * 0.5f);		// �Ͼ������ �Ÿ� * ź��Ʈ �ݽþ߰� (-1 1�� �ٲ��ֱ� ���� 2����)
	float nearW = nearH * m_ratio;							// ȭ��� ���̸� ���ϴϱ� �ʺ� ����
	float scaleX = 2.f * m_near / nearW;					// 2 * n / w
	float scaleY = 2.f * m_near / nearH;					// 2 * n / h
	float scaleZ = m_far / (m_far - m_near);				// f / f - n
	float translate = -m_near * scaleZ;						// - nf / f - n

	// ���⼭ 2.f * near�� ������ ���������� �κ��̴� �����ص� �� ��.

	m_Proj = Matrix4x4
	(
		scaleX, 0.f, 0.f, 0.f,
		0.f, scaleY, 0.f, 0.f,
		0.f, 0.f, scaleZ, 1.f,
		0.f, 0.f, translate, 0.f
	);
	*/
}

void Camera::SetOrthographicView()
{
	isPerspective = false;
	/*
	float scaleX = 2.f / m_width;
	float scaleY = 2.f / m_height;
	float scaleZ = 1.f / (m_far - m_near);
	float translate = -m_near * scaleZ;

	m_Proj = Matrix4x4
	(
		scaleX, 0.f, 0.f, 0.f,
		0.f, scaleY, 0.f, 0.f,
		0.f, 0.f, scaleZ, 0.f,
		0.f, 0.f, translate, 1.f
	);
	*/
}
