#include "pch.h"
#include "UiRect.h"

#include "Rendering/Canvas.h"

UiRect UiRect::MakeChild(const Float2& parentPivot, const Float2& childPivot, const Float2& offset, const Float2& size) const
{
	UiRect child{ size, {}, Pivot };
	child.Offset -= (childPivot - Pivot) * size;
	child.Offset += (parentPivot - Pivot) * Size;
	child.Offset /= 2.f;
	child.Offset += offset + Offset;
	return child;
}

UiRect UiRect::GetShrinked(const Float2& amount) const
{
	return MakeChild({ 0,0 }, { 0,0 }, { 0,0 }, Size - amount);
}

Float2 UiRect::GetChildOffset(const Float2& parentPivot) const
{
	return (parentPivot - Pivot) * Size / 2 + Offset;
}

RectFloat UiRect::GetNdcRect() const
{
	Float2 leftBot{ GetChildOffset({ -1,-1 }) };
	leftBot /= CANVAS.GetSize();
	leftBot *= 2;
	leftBot += Pivot;

	Float2 size{ Size };
	size /= CANVAS.GetSize();
	size *= 2;

	return RectFloat{ leftBot, size };
}
