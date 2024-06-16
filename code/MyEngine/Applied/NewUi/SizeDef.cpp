#include "pch.h"
#include "SizeDef.h"

using namespace NewUi;

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
	else
		size.x = Value.x;

	if (VerticalMode == Mode::Percentage)
		size.y = parentSize.y * Value.y;
	else
		size.y = Value.y;

	return size;
}
