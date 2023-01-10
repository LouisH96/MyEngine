#pragma once
#include "../Input/Keyboard_.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class Keyboard_MsgListener
				: public Input::Keyboard_<Keyboard_MsgListener>
			{
			public:
				Keyboard_MsgListener();
				bool IsDown_(char c) const;

				void KeyUp(char c);
				void KeyDown(char c);

			private:
				static constexpr unsigned NR_KEYS = 256;
				static constexpr int PACK_SIZE = 8;
				static constexpr int NR_PACKS = NR_KEYS / PACK_SIZE;
				unsigned char m_Keys[NR_PACKS]{};
			};
		}
	}
}
