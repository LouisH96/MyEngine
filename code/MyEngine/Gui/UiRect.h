#pragma once

namespace MyEngine
{
	struct UiRect
	{
		Float2 Size; //in pixels
		Float2 Offset; //in pixels
		Float2 Pivot; //[-1,1]

		UiRect MakeChild(const Float2& parentPivot, const Float2& childPivot, const Float2& offset, const Float2& size) const;
		Float2 GetChildOffset(const Float2& parentPivot) const;
	};
}
