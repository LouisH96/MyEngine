#pragma once
#include "KeyCodes.h"
#include "Math/Constants.h"
#include "Math/Vectors.h"

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			template <class T>
			class Keyboard_
			{
			public:
				bool IsDown(char c) const;

				Math::Float2 GetWasdInput(float scale) const;
				Math::Int2 GetArrowInput() const;
				int GetInput(char positive, char negative) const;
			};

			template <class T>
			bool Keyboard_<T>::IsDown(char c) const
			{
				return reinterpret_cast<const T*>(this)->IsDown_(c);
			}

			template <class T>
			Math::Float2 Keyboard_<T>::GetWasdInput(float scale) const
			{
				Math::Float2 input;
				input.x = static_cast<float>(IsDown('D') - IsDown('Q')) * scale;
				input.y = static_cast<float>(IsDown('Z') - IsDown('S')) * scale;
				if (input.x != 0 && input.y != 0) input *= Math::Constants::DIVSQR2;
				return input;
			}

			template <class T>
			Math::Int2 Keyboard_<T>::GetArrowInput() const
			{
				return{
					IsDown(KeyCodes::ARROW_RIGHT) - IsDown(KeyCodes::ARROW_LEFT),
					IsDown(KeyCodes::ARROW_UP) - IsDown(KeyCodes::ARROW_DOWN) };
			}

			template <class T>
			int Keyboard_<T>::GetInput(char positive, char negative) const
			{
				return IsDown(positive) - IsDown(negative);
			}
		}
	}
}
