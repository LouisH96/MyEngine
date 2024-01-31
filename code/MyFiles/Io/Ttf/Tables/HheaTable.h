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
			class HheaTable
			{
			public:
				void Read(Bin::BigBinReader& reader);

				void Print() const;

				uint32_t Version{};
				int16_t Ascent{};
				int16_t Descent{};
				int16_t LineGap{};
				uint16_t AdvanceWidthMax{};
				int16_t MinLeftSideBearing{};
				int16_t MinRightSideBearing{};
				int16_t XMaxExtent{};
				int16_t CaretSlopeRise{};
				int16_t CaretSlopeRun{};
				int16_t CaretOffset{};
				int16_t Reserved[4]{};
				int16_t MetricDataFormat{};
				uint16_t NumOfLongHorMetrics{};
			};
		}
	}
}
