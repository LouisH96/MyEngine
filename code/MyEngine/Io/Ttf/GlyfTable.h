#pragma once
#include "TtfPoint.h"

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
			class GlyfTable
			{
			public:
				void Read(const Bin::BigBinReader& reader);
				Array<TtfPoint> GetContour(const Bin::BigBinReader& reader, uint32_t glyphOffset) const;

			private:
				//---| Types |---
				struct SimpleOutline
				{
					//--| Types |---
					union Flags
					{
						struct
						{
							uint8_t onCurve : 1;
							uint8_t xShort : 1;
							uint8_t yShort : 1;
							uint8_t repeat : 1;
							uint8_t xShortPos : 1;
							uint8_t yShortPos : 1;
							uint8_t reserved1 : 1;
							uint8_t reserved2 : 1;
						};
						uint8_t flag;
					};
					int16_t nrOfContours;
					int16_t xMin;
					int16_t yMin;
					int16_t xMax;
					int16_t yMax;
					uint16_t instructionLength;
				};

				//---| Members |---
				uint32_t m_Begin{};
			};
		}
	}
}
