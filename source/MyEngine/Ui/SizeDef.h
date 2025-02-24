#pragma once

namespace MyEngine
{
namespace Ui
{
struct SizeDef
{
	enum class Mode
	{
		Pixels, Percentage, PercentageOfOther //percentage of width or height
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

	static SizeDef Pixels(const Float2& size);
	static SizeDef Pixels(float width, float height);
	static SizeDef Percentages(const Float2& size);
	static SizeDef Percentages(float width, float height);
};
}
}
