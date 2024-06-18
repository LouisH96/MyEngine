#pragma once
#include <istream>

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
			class CMapTable
			{
			public:
				void Read(Bin::BigBinReader& reader);
				void Print(bool printSubTables = false) const;
				static void PrintHeader();

				uint16_t GetGlyphIndex(uint16_t codePoint, uint16_t platformId = 0, uint16_t platformSpecificId = 3) const;
				uint16_t GetCodePointFromIndex(uint16_t glyphIndex, uint16_t platformId = 0, uint16_t platformSpecificId = 3) const;

			private:
				//---| Types |---
				struct CMapFormat4
				{
					uint16_t format;
					uint16_t length;
					uint16_t language;
					uint16_t segCountX2;
					uint16_t searchRange;
					uint16_t entrySelector;
					uint16_t rangeShift;
					Array<uint16_t> endCode;
					Array<uint16_t> startCode;
					Array<uint16_t> idDelta;
					Array<uint16_t> idRangeOffset;
					Array<uint16_t> glyphIdArray;
				};
				class EncodingSubTable
				{
				public:
					void Read(Bin::BigBinReader& reader);
					void ReadFormat(Bin::BigBinReader& reader);
					void Print() const;
					static void PrintHeader();

					uint16_t GetGlyphIndex(uint16_t codePoint) const;
					uint16_t GetCodePoint(uint16_t glyphIndex) const;

					uint16_t GetPlatformId() const { return m_PlatformId; }
					uint16_t GetPlatformSpecificId() const { return m_PlatformSpecificId; }
					uint32_t GetOffset() const { return m_Offset; }

				private:
					uint16_t m_PlatformId{};
					uint16_t m_PlatformSpecificId{};
					uint32_t m_Offset{};
					CMapFormat4 m_Format{};
				};

				uint16_t m_VersionNumber{};
				Array<EncodingSubTable> m_SubTables{};

				void Validate() const;
			};
		}
	}
}
