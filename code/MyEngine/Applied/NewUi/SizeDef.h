#pragma once

namespace MyEngine
{
	namespace NewUi
	{
		struct SizeDef
		{
			enum class Mode
			{
				Pixels, Percentage
			};
			Mode HorizontalMode{ Mode::Percentage };
			Mode VerticalMode{ Mode::Percentage };
			Float2 Value{ 1.f,1.f };

			Float2 GetSize(const Float2& parentSize) const;
		};
	}
}
