#include "pch.h"
#include "SizeDef.h"

using namespace Ui;

SizeDef::SizeDef(Mode horizontalMode, Mode verticalMode, const Float2& value)
	: HorizontalMode{ horizontalMode }
	, VerticalMode{ verticalMode }
	, Value{ value }
{
}

SizeDef::SizeDef(Mode bothModes, const Float2& value)
	: HorizontalMode{ bothModes }
	, VerticalMode{ bothModes }
	, Value{ value }
{
}

void SizeDef::SetPixelsMode()
{
	SetModes(Mode::Pixels);
}

void SizeDef::SetPercentageModed()
{
	SetModes(Mode::Percentage);
}

void SizeDef::SetModes(Mode bothModes)
{
	HorizontalMode = bothModes;
	VerticalMode = bothModes;
}

Float2 SizeDef::GetSize(const Float2& parentSize) const
{
	Float2 size;

	if (HorizontalMode == Mode::Percentage)
		size.x = parentSize.x * Value.x;
	else if (HorizontalMode == Mode::Pixels)
		size.x = Value.x;

	if (VerticalMode == Mode::Percentage)
		size.y = parentSize.y * Value.y;
	else if (VerticalMode == Mode::Pixels)
		size.y = Value.y;
	else
		size.y = size.x * Value.y;

	if (HorizontalMode == Mode::PercentageOfOther)
		size.x = size.y * Value.x;

	return size;
}

SizeDef SizeDef::Pixels(const Float2& values)
{
	return { Mode::Pixels, values };
}

SizeDef SizeDef::Pixels(float width, float height)
{
	return { Mode::Pixels, {width, height} };
}

SizeDef SizeDef::Percentages(const Float2& values)
{
	return { Mode::Percentage, values };
}

SizeDef SizeDef::Percentages(float width, float height)
{
	return { Mode::Percentage, {width, height} };
}
