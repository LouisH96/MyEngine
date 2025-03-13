#include "GlyfTable.h"

#include <Io/Binary/BigBinReader.h>
#include <Math/Vector2.h>

#include "TtfReader.h"
#include "Logger/Logger.h"

using namespace MyEngine;
using namespace Io;
using namespace Ttf;

void GlyfTable::Read(Bin::BigBinReader& reader)
{
	m_Begin = reader.GetPos();
}

Array<Array<TtfPoint>> GlyfTable::GetContours(Bin::BigBinReader& reader, uint32_t glyphOffset, const TtfReader& ttfReader) const
{
	int16_t minX, minY, maxX, maxY;
	return GetContours(reader, glyphOffset, minX, maxX, minY, maxY, ttfReader);
}

Glyph GlyfTable::GetGlyph(Bin::BigBinReader& reader, uint32_t glyphOffset, const TtfReader& ttfReader) const
{
	int16_t minX, minY, maxX, maxY;
	const Array<Array<TtfPoint>> points{ GetContours(reader, glyphOffset, minX, maxX, minY, maxY, ttfReader) };
	const Double2 minBounds{ static_cast<double>(minX), static_cast<double>(minY) };
	const Double2 maxBounds{ static_cast<double>(maxX), static_cast<double>(maxY) };
	return Glyph{ points, minBounds, maxBounds };
}

GlyfTable::CompoundComponent::CompoundComponent(Bin::BigBinReader& reader)
{
	reader.Read(flag.asUint);
	reader.Read(glyphIndex);

	if (flag.bits.arg1And2AreWords)
	{
		if (flag.bits.argsAreXyValues)
		{
			argument1 = reader.Int16();
			argument2 = reader.Int16();
		}
		else
		{
			Logger::Error("[GlyfTable::GetCompoundGlyphContour] Point values not supported yet");
			argument1 = reader.Uint16();
			argument2 = reader.Uint16();
		}
	}
	else
	{
		if (flag.bits.argsAreXyValues)
		{
			argument1 = reader.Int8();
			argument2 = reader.Int8();
		}
		else
		{
			Logger::Error("[GlyfTable::GetCompoundGlyphContour] Point values not supported yet");
			argument1 = reader.Uint8();
			argument2 = reader.Uint8();
		}
	}

	if (flag.bits.weHaveInstructions)
	{
		Logger::Error("[GlyfTable::GetCompoundGlyphContour] instructions not supported yet");
	}

	transform[0] = 1;
	transform[1] = 0;
	transform[2] = 0;
	transform[3] = 1;

	if (flag.bits.weHaveAScale)
	{
		Logger::Error("[GlyfTable::GetCompoundGlyphContour] (A-Scale) transformation not supported yet");
		transform[0] = reader.Int16();
		transform[3] = transform[0];
	}
	else if (flag.bits.weHaveAnXAndYScale)
	{
		Logger::Error("[GlyfTable::GetCompoundGlyphContour] (X&Y-Scale) transformation not supported yet");
		transform[0] = reader.Int16();
		transform[3] = reader.Int16();
	}
	else if (flag.bits.weHaveATwoByTwo)
	{
		Logger::Error("[GlyfTable::GetCompoundGlyphContour] (TwoByTwo) transformation not supported yet");
		transform[0] = reader.Int16();
		transform[1] = reader.Int16();
		transform[2] = reader.Int16();
		transform[3] = reader.Int16();
	}
}

Array<Array<TtfPoint>> GlyfTable::GetContours(Bin::BigBinReader& reader, uint32_t glyphOffset,
	int16_t& minX, int16_t& maxX, int16_t& minY, int16_t& maxY, const TtfReader& ttfReader) const
{
	reader.SetPos(m_Begin + glyphOffset);
	const int16_t nrContours{ reader.Int16() };
	reader.Read(minX);
	reader.Read(minY);
	reader.Read(maxX);
	reader.Read(maxY);

	Array<Array<TtfPoint>> contours;

	if (nrContours > 0)
		contours = GetSimpleGlyphContour(reader, nrContours);
	else if (nrContours < 0)
		contours = GetCompoundGlyphContour(reader, nrContours, ttfReader);
	else
	{
		Logger::Warning("[GlyfTable::GetContours] No contour");
		contours = {};
	}
	return contours;
}

Array<Array<TtfPoint>> GlyfTable::GetSimpleGlyphContour(Bin::BigBinReader& reader, int16_t nrContours) const
{
	SimpleOutline outline;
	outline.nrOfContours = nrContours;

	Array<uint16_t> endPtsOfContours{ outline.nrOfContours };
	for (unsigned i = 0; i < endPtsOfContours.GetSize(); i++)
		endPtsOfContours[i] = reader.Uint16();

	//instructions
	const uint16_t nrInstructions{ reader.Uint16() };
	for (int i = 0; i < nrInstructions; i++)
		reader.Uint8();

	//
	const uint32_t lastIndex{ endPtsOfContours[outline.nrOfContours - 1] };

	//flags
	Array<SimpleOutline::Flags> allFlags{ lastIndex + 1 };
	for (unsigned iFlag = 0; iFlag < allFlags.GetSize(); )
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
		points.Last() = points.First();
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
		points.Last().position.y = points.First().position.y;
	}

	//Fix contours beginning with control point
	for (unsigned iContour{ 0 }; iContour < contours.GetSize(); ++iContour)
	{
		Array<TtfPoint>& points{ contours[iContour] };

		if (points.First().isOnCurve)
			continue;

		std::copy_backward(&points[0], &points[points.GetSize() - 1], &points[points.GetSize()]);
		points.First() = points.Last();
	}

	return contours;
}

Array<Array<TtfPoint>> GlyfTable::GetCompoundGlyphContour(Bin::BigBinReader& reader, int16_t nrContours, const TtfReader& ttfReader) const
{
	Array<Array<TtfPoint>> allContours{};

	CompoundComponent component;
	do
	{
		component = CompoundComponent{ reader };
		const auto readerPos{ reader.GetPos() };

		const uint32_t glyphOffset{ ttfReader.GetLocaTable().GetGlyphOffset(reader, component.glyphIndex) };
		Array<Array<TtfPoint>> compoundContours{ GetContours(reader, glyphOffset, ttfReader) };
		reader.SetPos(readerPos);

		//apply transformations & add to allContours
		for (unsigned iContour = 0; iContour < compoundContours.GetSize(); iContour++)
		{
			Array<TtfPoint>& points{ compoundContours[iContour] };

			for (unsigned iPoint = 0; iPoint < points.GetSize(); iPoint++)
			{
				TtfPoint& point{ points[iPoint] };
				point.position.x += component.argument1;
				point.position.y += component.argument2;
			}
			allContours.Add(std::move(points));
		}

	} while (component.flag.bits.moreComponents);

	return allContours;
}
