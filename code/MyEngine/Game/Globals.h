#pragma once
#include "App/Wrappers/Win32/Keyboard_MsgListener.h"
#include "App/Wrappers/Win32/Mouse.h"

namespace MyEngine
{
	class Globals
	{
	public:
		static float DeltaTime;
		static const App::Wrappers::Win32::Keyboard_MsgListener* pKeyboard;
		static const App::Wrappers::Win32::Mouse* pMouse;
	};
}
