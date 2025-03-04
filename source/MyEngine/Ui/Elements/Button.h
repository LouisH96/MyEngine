#pragma once
#include "..\Elem.h"
#include "..\GraphicsId.h"

namespace MyEngine
{
namespace Ui
{
class Button final
	: public Elem
{
public:
	using FunctionArg = unsigned;
	using Function = void (*)(FunctionArg);

	explicit Button(const std::string& text, Function function, FunctionArg functionArg, float fontSize = 18);

	void ToDefaultState() override;
	void ToHoverState() override;
	void ToPressedState() override;
	void OnClick() override;

	const std::string GetTypeName() const override;

	void SetBgColor(const Float3& color);
	void SetBorderColor(const Float3& color);
	void SetInteractColor(const Float3& color);

private:
	void UpdateSizeAndTreePositions(const ResizePref& pref) override;
	void Clear() override;
	void Create() override;

	static const Float2 MARGIN;

	std::string m_Text;
	float m_FontSize;
	RectId m_BorderId{};
	RectId m_BackgroundId{};
	FontId m_TextId{};
	Function m_Function;
	FunctionArg m_FunctionArg;
	Float3 m_BorderColor;
	Float3 m_BgColor;
	Float3 m_InteractColor;
};
}
}
