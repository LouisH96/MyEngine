#include "pch.h"
#include "GlyfTable.h"

#include <Io/Binary/BigBinReader.h>
#include <Math/Vector2.h>

void Io::Ttf::GlyfTable::Read(const Bin::BigBinReader& reader)
{
	m_Begin = reader.GetPos();
}

Array<Array<Io::Ttf::TtfPoint>> Io::Ttf::GlyfTable::GetContours(const Bin::BigBinReader& reader, uint32_t glyphOffset) const
{
	int16_t minX, minY, maxX, maxY;
	return GetContours(reader, glyphOffset, minX, maxX, minY, maxY);
}

Io::Ttf::Glyph Io::Ttf::GlyfTable::GetGlyph(const Bin::BigBinReader& reader, uint32_t glyphOffset) const
{
	int16_t minX, minY, maxX, maxY;
	const Array<Array<TtfPoint>> points{ GetContours(reader, glyphOffset, minX, maxX, minY, maxY) };
	const Math::Double2 minBounds{ static_cast<double>(minX), static_cast<double>(minY) };
	const Math::Double2 maxBounds{ static_cast<double>(maxX), static_cast<double>(maxY) };
	return Glyph{ points, minBounds, maxBounds };
}

Array<Array<Io::Ttf::TtfPoint>> Io::Ttf::GlyfTable::GetContours(const Bin::BigBinReader& reader, uint32_t glyphOffset,
                                                                int16_t& minX, int16_t& maxX, int16_t& minY, int16_t& maxY) const
{
	reader.SetPos(m_Begin + glyphOffset);
	SimpleOutline outline{};
	reader.Read(outline.nrOfContours);
	reader.Read(outline.xMin);
	reader.Read(outline.yMin);
	reader.Read(outline.xMax);
	reader.Read(outline.yMax);
	minX = outline.xMin;
	minY = outline.yMin;
	maxX = outline.xMax;
	maxY = outline.yMax;
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
	Array<Array<TtfPoint>> contours = { outline.nrOfContours };

	int16_t prevX = 0;
	int iFlag = 0;
	for (int iContour = 0; iContour < outline.nrOfContours; iContour++)
	{
		const int contourBegin = iContour == 0 ? 0 : endPtsOfContours[iContour - 1] + 1;
		const int contourEnd = endPtsOfContours[iContour];
		Array<TtfPoint>& points{ contours[iContour] };
		points = { contourEnd - contourBegin + 2 };

		for (int i = 0; i <= contourEnd - contourBegin; i++)
		{
			const SimpleOutline::Flags flag{ allFlags[iFlag++] };
			const uint8_t xFlags{ static_cast<uint8_t>((flag.xShort << 1) + flag.xShortPos) };
			switch (xFlags)
			{
			case 0: points[i].position.x = prevX + reader.Int16(); break;
			case 1: points[i].position.x = prevX; break;
			case 2: points[i].position.x = prevX - reader.Uint8(); break;
			case 3: points[i].position.x = prevX + reader.Uint8(); break;
			}
			prevX = points[i].position.x;
			points[i].isOnCurve = flag.onCurve;
		}
		points[points.GetSize() - 1].position.x = points[0].position.x;
		points[points.GetSize() - 1].isOnCurve = true;
	}

	//yCoord
	int16_t prevY = 0;
	iFlag = 0;
	for (int iContour = 0; iContour < outline.nrOfContours; iContour++)
	{
		const int contourBegin = iContour == 0 ? 0 : endPtsOfContours[iContour - 1] + 1;
		const int contourEnd = endPtsOfContours[iContour];
		Array<TtfPoint>& points{ contours[iContour] };

		for (int i = 0; i <= contourEnd - contourBegin; i++)
		{
			const SimpleOutline::Flags flag{ allFlags[iFlag++] };
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
		points[points.GetSize() - 1].position.y = points[0].position.y;
	}
	return contours;
}
