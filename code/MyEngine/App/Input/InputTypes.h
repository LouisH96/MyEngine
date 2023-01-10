#pragma once
#include "Keyboard_.h"

//#include "Keyboard_StateReader.h"
#include "Keyboard_MsgListener.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			typedef Keyboard_<Keyboard_MsgListener> Keyboard;
		}
	}
}
