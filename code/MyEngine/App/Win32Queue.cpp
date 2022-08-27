#include "Win32Queue.h"
#include <Windows.h>

void MyEngine::App::Win32Queue::HandleMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			m_IsActive = false;
	}
}
