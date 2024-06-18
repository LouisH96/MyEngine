#pragma once
#include "Glyph.h"
#include "TtfPoint.h"

//named Glyf instead of Glyph because its the table's code inside the .ttf file

namespace MyEngine
{
	namespace Io
	{
		class TtfReader;

		namespace Bin
		{
			class BigBinReader;
		}

		namespace Ttf
		{
			class GlyfTable
			{
			public:
				void Read(Bin::BigBinReader& reader);
				Array<Array<TtfPoint>> GetContours(Bin::BigBinReader& reader, uint32_t glyphOffset, const TtfReader& ttfReader) const;
				Glyph GetGlyph(Bin::BigBinReader& reader, uint32_t glyphOffset, const TtfReader& ttfReader) const;

			private:
				//---| Types |---
				struct SimpleOutline
				{
					//--| Types |---
					union Flags
					{
#pragma warning( push )
#pragma warning( disable : 4201)
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
#pragma warning( pop ) 
						uint8_t flag;
					};
					int16_t nrOfContours;
					uint16_t instructionLength;
				};
				struct CompoundComponent
				{
					union Flag
					{
						uint16_t asUint;
						struct Bits
						{
							unsigned arg1And2AreWords : 1;
							unsigned argsAreXyValues : 1;
							unsigned roundXyToGrid : 1;
							unsigned weHaveAScale : 1;
							unsigned obsolete : 1;
							unsigned moreComponents : 1;
							unsigned weHaveAnXAndYScale : 1;
							unsigned weHaveATwoByTwo : 1;
							unsigned weHaveInstructions : 1;
							unsigned useMyMetrics : 1;
							unsigned overlapCompound : 1;
						};
						Bits bits;
					};
					Flag flag{};
					uint16_t glyphIndex{};
					int32_t argument1{};
					int32_t argument2{};
					int16_t transform[4]{};

					CompoundComponent() = default;
					explicit CompoundComponent(Bin::BigBinReader& reader);
				};

				//---| Members |---
				uint32_t m_Begin{};

				Array<Array<TtfPoint>> GetContours(Bin::BigBinReader& reader, uint32_t glyphOffset, int16_t& minX, int16_t& maxX, int16_t& minY, int16_t& maxY, const TtfReader& ttfReader) const;

				Array<Array<TtfPoint>> GetSimpleGlyphContour(Bin::BigBinReader& reader, int16_t nrContours) const;
				Array<Array<TtfPoint>> GetCompoundGlyphContour(Bin::BigBinReader& reader, int16_t nrContours, const TtfReader& ttfReader) const;
			};
		}
	}
}
