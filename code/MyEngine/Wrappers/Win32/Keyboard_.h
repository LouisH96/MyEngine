#pragma once

namespace MyEngine
{
	namespace Wrappers
	{
		namespace Win32
		{
			template <class T>
			class Keyboard_
			{
			public:
				bool IsDown(char c) const;
			};

			template <class T>
			bool Keyboard_<T>::IsDown(char c) const
			{
				return reinterpret_cast<const T*>(this)->IsDown_(c);
			}
		}
	}
}
