#pragma once

namespace MyEngine
{
	namespace NewUi
	{
		struct SizeDef
		{
			enum Mode
			{
				Pixels, Percentage
			};
			Mode HorizontalMode{ Percentage };
			Mode VerticalMode{ Percentage };
			Float2 Value{ 1.f,1.f };

			Float2 GetSize(const Float2& parentSize) const;
		};
	}
}
