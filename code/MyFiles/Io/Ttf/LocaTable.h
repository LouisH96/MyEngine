#pragma once
#include <cstdint>

namespace MyEngine
{
	namespace Io
	{
		namespace Bin
		{
			class BigBinReader;
		}

		namespace Ttf
		{
			class LocaTable
			{
			public:
				void Read(const Bin::BigBinReader& reader);
				void SetIsShortVersion(bool isShort);

				uint32_t GetGlyphOffset(const Bin::BigBinReader& reader, uint32_t glyphIdx) const;

			private:
				uint32_t m_Begin{};
				bool m_IsShortVersion{};
			};
		}
	}
}
