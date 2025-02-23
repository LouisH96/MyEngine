#include "pch.h"
#include "NdcUtils.h"

#include <Geometry\Shapes\Rects.h>
#include <Rendering\Canvas.h>

using namespace Gui;

Float2 NdcUtils::GetMouseNdc(const Float2& invCanvasSize)
{
	return ScreenPointToNdc(Globals::pMouse->GetPos(), invCanvasSize);
}

Float2 NdcUtils::ScreenPointToNdc(const Int2& point, const Float2& invCanvasSize)
{
	return Float2{ point } *Float2{ 2, -2 } *invCanvasSize - Float2{ 1, -1 };
}

Float2 NdcUtils::ScreenPointToNdc(const Float2& point, const Float2& invCanvasSize)
{
	return point * invCanvasSize * Float2{ 2,-2 } - Float2{ 1, -1 };
}

Float2 NdcUtils::GetInvSize(const Int2& size)
{
	return Float2{ 1 } / size;
}

Float2 NdcUtils::UpdateInvCanvasSize(const Int2& newSize, Float2& invCanvasSize)
{
	const Float2 oldInvCanvasSize{ invCanvasSize };
	invCanvasSize = Float2{ 1 } / newSize;
	return invCanvasSize / oldInvCanvasSize;
}

void NdcUtils::Resize(const Float2& scale, const Float2& pivot, Float2& rectCenter, Float2& rectSize)
{
	rectCenter -= pivot;
	rectCenter *= scale;
	rectCenter += pivot;
	rectSize *= scale;
}

RectFloat NdcUtils::ScreenRectToNdc(const RectFloat& rect)
{
	return ScreenRectToNdc(CANVAS.GetInvSize(), rect);
}

RectFloat NdcUtils::ScreenRectToNdc(Float2 invCanvasSize, const RectFloat& rect)
{
	invCanvasSize *= 2;
	return RectFloat{
		rect.GetLeftBot() * invCanvasSize - 1,
		rect.GetSize() * invCanvasSize
	};
}

void NdcUtils::ScreenRectToNdc(const Float2& invCanvasSize, const Float2& offset, const Float2& size,
	const Float2& pivot, Float2& rectCenter, Float2& rectSize)
{
	const Float2 halfNdcSize{ size * invCanvasSize };
	rectSize = halfNdcSize * 2;
	rectCenter = pivot; //pivot
	rectCenter -= halfNdcSize * pivot; //center of rect
	rectCenter += (offset * 2) * invCanvasSize; //offset applied
}

void NdcUtils::ScreenRectToNdc(const Float2& invCanvasSize, const Float2& canvasPivot, const Float2& offset,
	const Float2& size, const Float2& elementPivot, Float2& rectCenter, Float2& rectSize)
{
	const Float2 halfNdcSize{ size * invCanvasSize };
	rectSize = halfNdcSize * 2;
	rectCenter += canvasPivot + offset * 2 * invCanvasSize;
	rectCenter += halfNdcSize * elementPivot;
}

void NdcUtils::SetScreenSpaceOffset(const Float2& invCanvasSize, const Float2& pivot,
	const Float2& screenSpaceCenter, const Float2& rectSize, Float2& center)
{
	const Float2 localPivot{ rectSize * .5f * pivot }; //center of rect (0,0) to rects local pivot point
	const Float2 ndcOffset{ screenSpaceCenter * invCanvasSize * 2 };
	center = pivot - localPivot + ndcOffset;
}

void NdcUtils::SetScreenSpaceOffsetX(const Float2& invCanvasSize, const Float2& pivot, float screenSpaceX,
	const Float2& rectSize, Float2& center)
{
	const float localPivot{ rectSize.x * .5f * pivot.x };
	const float ndcOffset{ screenSpaceX * invCanvasSize.x * 2 };
	center.x = pivot.x - localPivot + ndcOffset;
}

void NdcUtils::SetScreenSpaceOffsetY(const Float2& invCanvasSize, const Float2& pivot, float screenSpaceY,
	const Float2& rectSize, Float2& center)
{
	const float localPivot{ rectSize.y * .5f * pivot.y };
	const float ndcOffset{ screenSpaceY * invCanvasSize.y * 2 };
	center.y = pivot.y - localPivot + ndcOffset;
}
