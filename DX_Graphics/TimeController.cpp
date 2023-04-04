#include "TimeController.h"


TimeController::TimeController() :
	currCount{},
	prevCount{},
	Frequency{},
	Acc{ 0.f },
	callCount{ 0 }
{
	QueryPerformanceCounter(&prevCount);
	QueryPerformanceFrequency(&Frequency);
}

TimeController::~TimeController()
{

}

TimeController& TimeController::GetInstance()
{
	static TimeController instance;
	return instance;
}

void TimeController::Update()
{
	QueryPerformanceCounter(&currCount);

	dt = (double)(currCount.QuadPart - prevCount.QuadPart) / (double)(Frequency.QuadPart);

	prevCount = currCount;
	++(callCount);
	Acc += dt;

	if (Acc >= 1.0f)
	{
		FPS = callCount;
		Acc = 0.f;
		callCount = 0;
	}
}
