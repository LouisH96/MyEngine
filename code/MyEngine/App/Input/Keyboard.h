#pragma once

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class Keyboard
			{
			public:

				void Update();
				bool IsDown(unsigned char c) const;

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

