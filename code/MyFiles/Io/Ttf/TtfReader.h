#pragma once
//https://handmade.network/forums/articles/t/7330-implementing_a_font_reader_and_rasterizer_from_scratch%252C_part_1__ttf_font_reader.

#include <Io/Binary/BigBinReader.h>
#include "TtfPoint.h"
#include "CMapTable.h"
#include "GlyfTable.h"
#include "Glyph.h"
#include "HeadTable.h"
#include "LocaTable.h"
#include "OffsetSubTable.h"
#include "TableDirectory.h"

namespace MyEngine
{
	namespace Io
	{
		class TtfReader
		{
		public:
			explicit TtfReader(std::istream& stream);
			~TtfReader() = default;
			Array<Array<Ttf::TtfPoint>> GetPoints(unsigned char c);

			Ttf::Glyph GetGlyph(char character);
			Ttf::Glyph GetGlyphFromIndex(uint16_t glyphIndex);

			const Ttf::CMapTable& GetCMapTable() const { return m_CMapTable; }
			const Ttf::LocaTable& GetLocaTable() const { return m_LocaTable; }

		private:
			//---| Members |---
			Bin::BigBinReader m_Reader;

			Ttf::OffsetSubTable m_OffsetSubTable;
			Array<Ttf::TableDirectory> m_TableDirectories;
			Ttf::HeadTable m_HeadTable{};
			Ttf::CMapTable m_CMapTable{};
			Ttf::LocaTable m_LocaTable{};
			Ttf::GlyfTable m_GlyfTable{};
			
			void ReadTableDirectories();
			void ReadTables();
		};
	}
}
