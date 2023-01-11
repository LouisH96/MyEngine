#pragma once
#include "Keyboard_.h"

namespace MyEngine
{
	namespace Wrappers
	{
		namespace Win32
		{
			class Keyboard_StateReader
				: public Keyboard_<Keyboard_StateReader>
			{
			public:
				void Update();
				bool IsDown_(unsigned char c) const;

			private:
				static constexpr unsigned NR_KEYS = 256;
				static constexpr int PACK_SIZE = 8;
				static constexpr int NR_PACKS = NR_KEYS / PACK_SIZE;
				unsigned char m_Keys[NR_PACKS]{};

				static void Pack(const unsigned char* pSource, unsigned char* pDest);
			};
		}
	}
}

