#include "CMapTable.h"

#include "Io/Binary/BigBinReader.h"
#include "Logger/Logger.h"

void MyEngine::Io::Ttf::CMapTable::Read(const Bin::BigBinReader& reader)
{
	const uint32_t begin{ reader.GetPos() };
	reader.Read(m_VersionNumber);
	const uint16_t nrSubTables{ reader.Uint16() };
	m_SubTables = { nrSubTables };
	for (int i = 0; i < m_SubTables.GetSize(); i++)
		m_SubTables[i].Read(reader);
	for (int i = 0; i < m_SubTables.GetSize(); i++)
	{
		reader.SetPos(begin + m_SubTables[i].GetOffset());
		m_SubTables[i].ReadFormat(reader);
	}
	Validate();
}

void MyEngine::Io::Ttf::CMapTable::Print(bool printSubTables) const
{
	std::cout << m_VersionNumber << "\t|";
	std::cout << m_SubTables.GetSize() << std::endl;
	if (!printSubTables)return;
	EncodingSubTable::PrintHeader();
	for (int i = 0; i < m_SubTables.GetSize(); i++)
		m_SubTables[i].Print();
}

void MyEngine::Io::Ttf::CMapTable::PrintHeader()
{
	std::cout << "___| CMapTable |___\n";
	std::cout << "Version\t|NumberSubTables\n";
}

uint16_t MyEngine::Io::Ttf::CMapTable::GetGlyphIndex(uint16_t codePoint, uint16_t platformId, uint16_t platformSpecificId) const
{
	for (int i = 0; i < m_SubTables.GetSize(); i++)
		if (m_SubTables[i].GetPlatformId() == platformId && m_SubTables[i].GetPlatformSpecificId() == platformSpecificId)
			return m_SubTables[i].GetGlyphIndex(codePoint);
	return -1;
}

void MyEngine::Io::Ttf::CMapTable::EncodingSubTable::Read(const Bin::BigBinReader& reader)
{
	reader.Read(m_PlatformId);
	reader.Read(m_PlatformSpecificId);
	reader.Read(m_Offset);
}

void MyEngine::Io::Ttf::CMapTable::EncodingSubTable::ReadFormat(const Bin::BigBinReader& reader)
{
	reader.Read(m_Format.format);
	if (m_Format.format != 4) return;
	reader.Read(m_Format.length);
	reader.Read(m_Format.language);
	reader.Read(m_Format.segCountX2);
	reader.Read(m_Format.searchRange);
	reader.Read(m_Format.entrySelector);
	reader.Read(m_Format.rangeShift);
	m_Format.endCode = { m_Format.segCountX2 / 2 };
	m_Format.startCode = { m_Format.segCountX2 / 2 };
	m_Format.idDelta = { m_Format.segCountX2 / 2 };
	m_Format.idRangeOffset = { m_Format.segCountX2 / 2 };
	for (int i = 0; i < m_Format.endCode.GetSize(); i++)
		m_Format.endCode[i] = reader.Uint16();
	reader.Uint16();
	for (int i = 0; i < m_Format.startCode.GetSize(); i++)
		m_Format.startCode[i] = reader.Uint16();
	for (int i = 0; i < m_Format.idDelta.GetSize(); i++)
		m_Format.idDelta[i] = reader.Uint16();
	for (int i = 0; i < m_Format.idRangeOffset.GetSize(); i++)
		m_Format.idRangeOffset[i] = reader.Uint16();
	const uint32_t currPos = 2 * 7 + 2 * 4 * m_Format.endCode.GetSize();
	m_Format.glyphIdArray = { (m_Format.length - currPos) / 2 };
	for (int i = 0; i < m_Format.glyphIdArray.GetSize(); i++)
		m_Format.glyphIdArray[i] = reader.Uint16();
}

void MyEngine::Io::Ttf::CMapTable::EncodingSubTable::Print() const
{
	std::cout << m_PlatformId << "\t|";
	std::cout << m_PlatformSpecificId << "\t|";
	std::cout << m_Offset << std::endl;
}

void MyEngine::Io::Ttf::CMapTable::EncodingSubTable::PrintHeader()
{
	std::cout << "___| CMapTable::EncodingSubTable |___\n";
	std::cout << "PlatformID\t|PlatformSpecificID\t|Offset\n";
}

uint16_t MyEngine::Io::Ttf::CMapTable::EncodingSubTable::GetGlyphIndex(uint16_t codePoint) const
{
	for (int i = 0; i < m_Format.endCode.GetSize(); i++)
	{
		const uint16_t endCode = m_Format.endCode[i];
		if (endCode < codePoint) continue;
		const uint16_t startCode = m_Format.startCode[i];
		if (startCode > codePoint)
		{
			Logger::PrintError("GlyphIndex not in table");
			return -1;
		}
		const uint16_t idRangeOffset = m_Format.idRangeOffset[i];
		if (idRangeOffset == 0)
			return codePoint + m_Format.idDelta[i];
		const int idx = idRangeOffset / 2 + i + (codePoint - startCode) - m_Format.idRangeOffset.GetSize();
		return m_Format.glyphIdArray[idx];
	}
	Logger::PrintError("GlyphIndex not found, but should have been");
	return -1;
}

void MyEngine::Io::Ttf::CMapTable::Validate() const
{
	for (int i = 0; i < m_SubTables.GetSize(); i++)
		if (m_SubTables[i].GetPlatformId() == 0 && m_SubTables[i].GetPlatformSpecificId() == 3)
			return;
	Logger::PrintError("SubTable with PlatformID 0 & PlatformSpecificID 3 not found");
}
