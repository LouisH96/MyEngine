#include "HeadTable.h"

#include "Io/Binary/BigBinReader.h"
#include "Logger/Logger.h"

void MyEngine::Io::Ttf::HeadTable::Read(const Bin::BigBinReader& reader)
{
	reader.Read(m_VersionInteger);
	reader.Read(m_VersionDecimal);
	reader.Read(m_FontRevisionInteger);
	reader.Read(m_FontRevisionDecimal);

	reader.Read(m_CheckSumAdjustment);
	reader.Uint32();//magic number

	reader.Read(m_Flags);
	reader.Read(m_UnitsPerEm);

	reader.Read(m_Created);
	reader.Read(m_Modified);

	reader.Read(m_XMin);
	reader.Read(m_YMin);
	reader.Read(m_XMax);
	reader.Read(m_YMax);

	reader.Read(m_MacStyle);
	reader.Read(m_LowestRecPpEm);
	reader.Read(m_FontDirectionHint);
	reader.Read(m_IndexToLocFormat);
	reader.Read(m_GlyphDataFormat);

	Validate();
}

void MyEngine::Io::Ttf::HeadTable::Validate() const
{
	if(m_IndexToLocFormat != 0)
	{
		Logger::PrintError("IndexToLocFormat is not 0, which is not supported");
	}
}
