#include "pch.h"
#include "NdcUtils.h"


Float2 Gui::NdcUtils::GetMouseNdc(const Float2& invCanvasSize)
{
	return ScreenPointToNdc(Globals::pMouse->GetPos(), invCanvasSize);
}

Float2 Gui::NdcUtils::ScreenPointToNdc(const Int2& point, const Float2& invCanvasSize)
{
	return Float2{ point.Scaled({2,-2}) }.Scaled(invCanvasSize) - Float2{1, -1};
}

Float2 Gui::NdcUtils::ScreenPointToNdc(const Float2& point, const Float2& invCanvasSize)
{
	return point.Scaled(invCanvasSize).Scaled({ 2,-2 }) - Float2{1, -1};
}

Float2 Gui::NdcUtils::GetInvSize(const Int2& size)
{
	return Float2{ 1 }.Divided(size);
}

Float2 Gui::NdcUtils::UpdateInvCanvasSize(const Int2& newSize, Float2& invCanvasSize)
{
	const Float2 oldInvCanvasSize{ invCanvasSize };
	invCanvasSize = Float2{ 1 }.Divided(Float2{ newSize });
	return invCanvasSize.Divided(oldInvCanvasSize);
}

void Gui::NdcUtils::Resize(const Float2& scale, const Float2& pivot, Float2& rectCenter, Float2& rectSize)
{
	rectCenter -= pivot;
	rectCenter.Scale(scale);
	rectCenter += pivot;
	rectSize.Scale(scale);
}

void Gui::NdcUtils::ScreenRectToNdc(const Float2& invCanvasSize, const Float2& offset, const Float2& size,
	const Float2& pivot, Float2& rectCenter, Float2& rectSize)
{
	const Float2 halfNdcSize{ size.Scaled(invCanvasSize) };
	rectSize = halfNdcSize * 2;
	rectCenter = pivot; //pivot
	rectCenter -= halfNdcSize.Scaled(pivot); //center of rect
	rectCenter += (offset * 2).Scaled(invCanvasSize); //offset applied
}

void Gui::NdcUtils::SetScreenSpaceOffset(const Float2& invCanvasSize, const Float2& pivot,
	const Float2& screenSpaceCenter, const Float2& rectSize, Float2& center)
{
	const Float2 localPivot{ rectSize.Scaled(.5f).Scaled(pivot) }; //center of rect (0,0) to rects local pivot point
	const Float2 ndcOffset{ screenSpaceCenter.Scaled(invCanvasSize) * 2 };
	center = pivot - localPivot + ndcOffset;
}

void Gui::NdcUtils::SetScreenSpaceOffsetX(const Float2& invCanvasSize, const Float2& pivot, float screenSpaceX,
	const Float2& rectSize, Float2& center)
{
	const float localPivot{ rectSize.x * .5f * pivot.x };
	const float ndcOffset{ screenSpaceX * invCanvasSize.x * 2 };
	center.x = pivot.x - localPivot + ndcOffset;
}

void Gui::NdcUtils::SetScreenSpaceOffsetY(const Float2& invCanvasSize, const Float2& pivot, float screenSpaceY,
	const Float2& rectSize, Float2& center)
{
	const float localPivot{ rectSize.y * .5f * pivot.y };
	const float ndcOffset{ screenSpaceY * invCanvasSize.y * 2 };
	center.y = pivot.y - localPivot + ndcOffset;
}
