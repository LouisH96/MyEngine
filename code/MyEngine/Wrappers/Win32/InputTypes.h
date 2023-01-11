#pragma once
#include "Keyboard_.h"
#include "Keyboard_MsgListener.h"

namespace MyEngine
{
	namespace Wrappers
	{
		namespace Win32
		{
			typedef Keyboard_<Keyboard_MsgListener> Keyboard;
		}
	}
}
