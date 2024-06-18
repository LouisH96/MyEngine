#pragma once
#include "../Win32/Keyboard_.h"

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Keyboard_MsgListener
				: public Keyboard_<Keyboard_MsgListener>
			{
			public:
				Keyboard_MsgListener();

				void PreInput();

				void KeyUp(char c);
				void KeyDown(char c);

				bool IsDown_(char c) const;
				bool IsPressed(char c) const;
				bool IsReleased(char c) const;

				Int2 GetPressedArrowInput() const;

			private:
				static constexpr unsigned NR_KEYS = 256;
				static constexpr int PACK_SIZE = 8;
				static constexpr int PACKS_PER_STATE = NR_KEYS / PACK_SIZE;
				static constexpr int NR_STATES = 2; //first: is currently down?, second: was down?
				static constexpr int TOTAL_PACKS = PACKS_PER_STATE * NR_STATES;
				unsigned char m_Keys[TOTAL_PACKS]{};
			};
		}
	}
}
