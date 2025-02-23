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

	SizeDef() = default;
	SizeDef(Mode horizontalMode, Mode verticalMode, const Float2& value);
	SizeDef(Mode mode, const Float2& value);

	void SetPixelsMode();
	void SetPercentageModed();
	void SetModes(Mode bothModes);
	Float2 GetSize(const Float2& parentSize) const;
};
}
}
