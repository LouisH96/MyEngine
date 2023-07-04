#include "LocaTable.h"

#include "Io/Binary/BigBinReader.h"

void MyEngine::Io::Ttf::LocaTable::Read(const Bin::BigBinReader& reader)
{
	m_Begin = reader.GetPos();
}

void MyEngine::Io::Ttf::LocaTable::SetIsShortVersion(bool isShort)
{
	m_IsShortVersion = isShort;
}

uint32_t MyEngine::Io::Ttf::LocaTable::GetGlyphOffset(const Bin::BigBinReader& reader, uint32_t glyphIdx) const
{
	if (m_IsShortVersion)
	{
		reader.SetPos(m_Begin + glyphIdx * 2);
		return reader.Uint16() * 2;
	}
	reader.SetPos(m_Begin + glyphIdx * 4);
	return reader.Uint32();
}
