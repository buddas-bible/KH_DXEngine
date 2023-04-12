#pragma once

#include "windows.h"

class TimeManager
{
private:
	TimeManager();
	~TimeManager();

	LARGE_INTEGER currCount;
	LARGE_INTEGER prevCount;
	LARGE_INTEGER Frequency;

	double dt;		// deltatime
	double Acc;		// 1초 체크를 위한 누적 시간

	int callCount;	// 함수 호출 횟수
	unsigned int FPS;		// 초당 호출된 횟수

public:
	static TimeManager& GetInstance();

	void Update();

	double GetDeltaTime() { return dt; };
	float GetfDeltaTime() { return (float)dt; };
	unsigned int GetFPS() { return FPS; };
};

