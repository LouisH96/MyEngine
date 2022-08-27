#include "Win32MsgHandler.h"
#include <Windows.h>

void MyEngine::App::Win32MsgHandler::HandleMessages()
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
