#pragma once
#include "Keyboard_.h"

//#include "Keyboard_StateReader.h"
#include "../Win32/Keyboard_MsgListener.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			typedef Keyboard_<Window::Keyboard_MsgListener> Keyboard;
		}
	}
}
