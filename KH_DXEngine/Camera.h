#pragma once
#include "Vector3D.h"

/// <summary>
/// ī�޶� Ŭ����
/// 
/// ī�޶��� z�� �̿��ؼ� xy ������ ���ϰ�
/// �� ��Ʈ������ �����ϰ� ������.
/// ���� ���� ����
/// </summary>
class Camera
{
public:
	Camera();
	~Camera();

private:
	Vector3D m_position;
	Vector3D m_cameraZ;
};


// �����غ��� FPS �帣�� ���ӵ��� z �� ȸ���� ���ߴ��� ����