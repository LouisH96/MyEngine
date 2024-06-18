#include "HmtxTable.h"

#include "HheaTable.h"
#include "Io/Binary/BigBinReader.h"

using namespace MyEngine::Io::Ttf;

void HmtxTable::Read(const Bin::BigBinReader& reader, const HheaTable& hheaTable)
{
	m_HMetrics = { hheaTable.NumOfLongHorMetrics };
	m_BeginLeftSideBearings = reader.GetPos() + m_HMetrics.GetSize() * sizeof(LongHorMetric);

	for (unsigned i = 0; i < m_HMetrics.GetSize(); i++)
	{
		reader.Read(m_HMetrics[i].AdvanceWidth);
		reader.Read(m_HMetrics[i].LeftSideBearing);
	}
}

HmtxTable::LongHorMetric HmtxTable::GetHorMetrics(Bin::BigBinReader& reader, uint16_t glyphIndex) const
{
	if (glyphIndex <= m_HMetrics.GetSize())
		return m_HMetrics[glyphIndex];

	//read it
	//return {};

	uint32_t pos{ m_BeginLeftSideBearings };
	pos += (glyphIndex - m_HMetrics.GetSize()) * sizeof(int16_t);
	reader.SetPos(pos);

	return LongHorMetric{
		m_HMetrics.Last().AdvanceWidth,
		reader.Int16()
	};
}
