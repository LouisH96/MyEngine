#include "TtfReader.h"

#include <fstream>

using namespace MyEngine;
using namespace Io;
using namespace Ttf;

TtfReader::TtfReader(std::istream& stream)
	: m_Reader{ stream }
{
	m_OffsetSubTable.Read(m_Reader);
	ReadTableDirectories();
	ReadTables();
	m_LocaTable.SetIsShortVersion(m_HeadTable.GetIndexToLocFormat() == 0);
}

Array<Array<TtfPoint>> TtfReader::GetPoints(unsigned char c) const
{
	const uint32_t aGlyphIndex = m_CMapTable.GetGlyphIndex(c);
	const uint32_t aGlyphOffset = m_LocaTable.GetGlyphOffset(m_Reader, aGlyphIndex);
	return m_GlyfTable.GetContours(m_Reader, aGlyphOffset);
}

Glyph TtfReader::GetGlyph(char character) const
{
	const uint16_t glyphIndex = m_CMapTable.GetGlyphIndex(character);
	if (glyphIndex == Scalar<decltype(glyphIndex)>::Max())
		return Glyph{};
	const uint32_t glyphOffset = m_LocaTable.GetGlyphOffset(m_Reader, glyphIndex);
	return m_GlyfTable.GetGlyph(m_Reader, glyphOffset);
}

void TtfReader::ReadTableDirectories()
{
	m_TableDirectories = { m_OffsetSubTable.GetNrTables() };
	for (unsigned i = 0; i < m_TableDirectories.GetSize(); i++)
		m_TableDirectories[i].Read(m_Reader);
}

void TtfReader::ReadTables()
{
	for (unsigned i = 0; i < m_TableDirectories.GetSize(); i++)
	{
		const std::string tag{ m_TableDirectories[i].GetTag() };
		m_Reader.SetPos(m_TableDirectories[i].GetOffset());
		if (tag == "head")
		{
			m_HeadTable.Read(m_Reader);
			continue;
		}
		if (tag == "cmap")
		{
			m_CMapTable.Read(m_Reader);
			continue;
		}
		if (tag == "loca")
		{
			m_LocaTable.Read(m_Reader);
			continue;
		}
		if (tag == "glyf")
		{
			m_GlyfTable.Read(m_Reader);
			continue;
		}
	}
}