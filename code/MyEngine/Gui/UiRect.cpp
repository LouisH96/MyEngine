#include "pch.h"
#include "UiRect.h"

UiRect UiRect::MakeChild(const Float2& parentPivot, const Float2& childPivot, const Float2& offset, const Float2& size) const
{
	UiRect child{ size, {}, Pivot };
	child.Offset -= (childPivot - Pivot) * size;
	child.Offset += (parentPivot - Pivot) * Size;
	child.Offset /= 2.f;
	child.Offset += offset + Offset;
	return child;
}

Float2 UiRect::GetChildOffset(const Float2& parentPivot) const
{
	return (parentPivot - Pivot) * Size / 2 + Offset;
}
