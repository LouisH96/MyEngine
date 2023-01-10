#pragma once
#include "Keyboard_.h"

#include "Keyboard_StateReader.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			typedef Keyboard_<Keyboard_StateReader> Keyboard;
		}
	}
}
