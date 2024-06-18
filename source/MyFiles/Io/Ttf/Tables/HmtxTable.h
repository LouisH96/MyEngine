#pragma once
#include <cstdint>

#include "DataStructures/Array.h"

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
			class HheaTable;

			class HmtxTable
			{
			public:
				struct LongHorMetric
				{
					uint16_t AdvanceWidth;
					int16_t LeftSideBearing;
				};

				void Read(const Bin::BigBinReader& reader, const HheaTable& hheaTable);
				LongHorMetric GetHorMetrics(Bin::BigBinReader& reader, uint16_t glyphIndex) const;

			private:
				uint32_t m_BeginLeftSideBearings{};
				Array<LongHorMetric> m_HMetrics;
				Array<int16_t> m_LeftSideBearings;
			};
		}
	}
}
