#include "pch.h"
#include "SizeDef.h"

using namespace NewUi;

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
