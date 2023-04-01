#include "pch.h"
#include "GlyfTable.h"

#include <Io/Binary/BigBinReader.h>
#include <Math/Value2.h>

void Io::Ttf::GlyfTable::Read(const Bin::BigBinReader& reader)
{
	m_Begin = reader.GetPos();
}

Array<Io::Ttf::TtfPoint> Io::Ttf::GlyfTable::GetContour(const Bin::BigBinReader& reader, uint32_t glyphOffset) const
{
	reader.SetPos(m_Begin + glyphOffset);
	SimpleOutline outline{};
	reader.Read(outline.nrOfContours);
	reader.Read(outline.xMin);
	reader.Read(outline.yMin);
	reader.Read(outline.xMax);
	reader.Read(outline.yMax);
	if (outline.nrOfContours < 0)
	{
		Logger::PrintError("CompoundGlyph not supported");
		return{};
	}

	Array<uint16_t> endPtsOfContours{ outline.nrOfContours };
	for (int i = 0; i < endPtsOfContours.GetSize(); i++)
		endPtsOfContours[i] = reader.Uint16();

	//instructions
	const uint16_t nrInstructions{ reader.Uint16() };
	for (int i = 0; i < nrInstructions; i++)
		reader.Uint8();

	//
	const uint32_t lastIndex{ endPtsOfContours[outline.nrOfContours - 1] };

	//flags
	Array<SimpleOutline::Flags> allFlags{ lastIndex + 1 };
	for (int iFlag = 0; iFlag < allFlags.GetSize(); )
	{
		const uint8_t intFlag{ reader.Uint8() };
		SimpleOutline::Flags flag{};
		flag.flag = intFlag;
		uint8_t nrStore = 1;
		if (flag.repeat)
			nrStore += reader.Uint8();
		for (int iRepeat = 0; iRepeat < nrStore; iRepeat++)
			allFlags[iFlag++] = flag;
	}

	//xCoord
	Array<TtfPoint> points = { allFlags.GetSize() };
	int16_t prevX = 0;
	for (int i = 0; i < points.GetSize(); i++)
	{
		const SimpleOutline::Flags flag{ allFlags[i] };
		const uint8_t xFlags{ static_cast<uint8_t>((flag.xShort << 1) + flag.xShortPos) };
		switch (xFlags)
		{
		case 1: points[i].position.x = prevX; break;
		case 2: points[i].position.x = prevX - reader.Uint8(); break;
		case 3: points[i].position.x = prevX + reader.Uint8(); break;
		case 0: points[i].position.x = prevX + reader.Int16(); break;
		}
		prevX = points[i].position.x;
		points[i].isControlPoint = !flag.onCurve;
	}
	//yCoord
	int16_t prevY = 0;
	for (int i = 0; i < points.GetSize(); i++)
	{
		const SimpleOutline::Flags flag{ allFlags[i] };
		const uint8_t yFlags{ static_cast<uint8_t>((flag.yShort << 1) + flag.yShortPos) };
		switch (yFlags)
		{
		case 0: points[i].position.y = prevY + reader.Int16(); break;
		case 1: points[i].position.y = prevY; break;
		case 2: points[i].position.y = prevY - reader.Uint8(); break;
		case 3: points[i].position.y = prevY + reader.Uint8(); break;
		}
		prevY = points[i].position.y;
	}
	return points;
}
