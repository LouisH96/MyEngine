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
			class OffsetSubTable
			{
			public:
				void Read(const Bin::BigBinReader& reader);
				uint16_t GetNrTables() const { return m_NrTables; }

			private:
				uint16_t m_NrTables{};
				static void ValidateType(uint32_t type);
			};
		}
	}
}
