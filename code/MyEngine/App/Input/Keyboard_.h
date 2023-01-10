#pragma once

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			template <class T>
			class Keyboard_
			{
			public:
				bool IsDown(char c);
			};

			template <class T>
			bool Keyboard_<T>::IsDown(char c)
			{
				return reinterpret_cast<T*>(this)->IsDown_(c);
			}
		}
	}
}
