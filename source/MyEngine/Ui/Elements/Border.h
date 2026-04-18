#pragma once
#include "../EmptyChildOptions.h"
#include "../GraphicsId.h"
#include "../ParentElem.h"

namespace MyEngine::Ui
{
class Border
	: public ParentElem<EmptyChildOptions>
{
public:
	Border(float border = UiSystem::BORDER_THICKNESS, float innerMargin = UiSystem::BORDER_THICKNESS);
	const std::string GetTypeName() const override { return "Border"; }

	void SetBorderColor(const Float3& borderColor);
	void SetBackgroundColor(const Float3& backgroundColor);

private:
	void TreeUpdate(const ResizePref& pref) override;
	void Clear() override;
	void Create() override;

	RectId m_BorderId{};
	RectId m_BackgroundId{};
	Float3 m_BorderColor;
	Float3 m_BackgroundColor;
	float m_BorderThickness;
	float m_Margin;
};
}
