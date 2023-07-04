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
			class HeadTable
			{
			public:
				void Read(const Bin::BigBinReader& reader);

				int16_t GetIndexToLocFormat() const { return m_IndexToLocFormat; }

			private:
				uint16_t m_VersionInteger{};
				uint16_t m_VersionDecimal{};
				uint16_t m_FontRevisionInteger{};
				uint16_t m_FontRevisionDecimal{};
				uint32_t m_CheckSumAdjustment{};
				uint16_t m_Flags{};
				uint16_t m_UnitsPerEm{};
				int64_t m_Created{};
				int64_t m_Modified{};
				int16_t m_XMin{};
				int16_t m_YMin{};
				int16_t m_XMax{};
				int16_t m_YMax{};
				uint16_t m_MacStyle{};
				uint16_t m_LowestRecPpEm{}; //smallest readable size in pixels
				int16_t m_FontDirectionHint{};
				int16_t m_IndexToLocFormat{};
				int16_t m_GlyphDataFormat{};

				void Validate() const;
			};
		}
	}
}
