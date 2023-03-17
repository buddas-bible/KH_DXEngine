#pragma once
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4x4.h"

/// <summary>
/// ī�޶� Ŭ����
/// 
/// ī�޶��� z�� �̿��ؼ� xy ���͸� ���ϰ�
/// �� ��Ʈ������ �����ϰ� ������.
/// ���� ���� ��Ʈ����..
/// </summary>
class Camera
{
public:
	Camera();
	~Camera();

private:
	Vector3D m_position;
	Vector3D m_cameraZ;

	float m_near;
	float m_far;

	Matrix4x4 m_viewMatrix;
	Matrix4x4 m_projectionMatrix;

public:
	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetProjectionMatrix();
};


// �����غ��� FPS �帣�� ���ӵ��� z �� ȸ���� ���ߴ��� ����