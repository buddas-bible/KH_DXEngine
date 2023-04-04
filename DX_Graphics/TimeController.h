#pragma once

#include "windows.h"

class TimeController
{
private:
	TimeController();
	~TimeController();

	LARGE_INTEGER currCount;
	LARGE_INTEGER prevCount;
	LARGE_INTEGER Frequency;

	double dt;		// deltatime
	double Acc;		// 1�� üũ�� ���� ���� �ð�

	int callCount;	// �Լ� ȣ�� Ƚ��
	unsigned int FPS;		// �ʴ� ȣ��� Ƚ��

public:
	static TimeController& GetInstance();

	void Update();

	double GetDeltaTime() { return dt; };
	float GetfDeltaTime() { return (float)dt; };
	unsigned int GetFPS() { return FPS; };
};

