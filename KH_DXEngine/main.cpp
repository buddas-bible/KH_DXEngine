#include <windows.h>

#include "GameLoop.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HRESULT hr;

	GameLoop loop;
	hr = loop.Initialize(hInstance);

	if (FAILED(hr))	return 0;

	loop.Loop();
	loop.Finalize();

	return 0;
}