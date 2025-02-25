#include "OffsetSubTable.h"

#include <Io/Binary/BigBinReader.h>

#include "Logger/Logger.h"

void MyEngine::Io::Ttf::OffsetSubTable::Read(const Bin::BigBinReader& reader)
{
	const uint32_t scalarType{ reader.Uint32() };
	ValidateType(scalarType);
	reader.Read(m_NrTables);
	const uint16_t searchRange{ reader.Uint16() };
	const uint16_t entrySelector{ reader.Uint16() };
	const uint16_t rangeShift{ reader.Uint16() };
}

void MyEngine::Io::Ttf::OffsetSubTable::ValidateType(uint32_t type)
{
	if (type == 0x74'72'75'65 ||
		type == 0x00'01'00'00)
		return;
	if (type == 0x4F'54'54'4F)
	{
		Logger::Error("TTF format is OpenType with PostScript outlines, which is not supported");
		return;
	}
	if (type == 0x74'79'70'31)
	{
		Logger::Error("TTF format is type1 postscript which is not supported");
		return;
	}
	Logger::Error("TTF format has unknown scalarType");
}
