#pragma once
#include "App/Win32/Keyboard_MsgListener.h"
#include "App/Win32/Mouse.h"

namespace MyEngine
{
	class Globals
	{
	public:
		static float DeltaTime;
		static const App::Win32::Keyboard_MsgListener* pKeyboard;
		static const App::Win32::Mouse* pMouse;
	};
}
