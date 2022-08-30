#include "Win32Messages.h"
#include <Windows.h>

void MyEngine::App::Win32::Win32Messages::HandleMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
