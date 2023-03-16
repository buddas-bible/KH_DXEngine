#pragma once
#include "Vector3D.h"

/// <summary>
/// 카메라 클래스
/// 
/// 카메라의 z를 이용해서 xy 기저를 구하고
/// 뷰 매트릭스를 유도하고 적용함.
/// 이후 투영 공간
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


// 생각해보면 FPS 장르의 게임들은 z 축 회전은 안했던거 같음