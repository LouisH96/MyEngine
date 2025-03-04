#pragma once
#include "..\EmptyChildOptions.h"
#include "..\GraphicsId.h"
#include "..\ParentElem.h"

namespace MyEngine
{
namespace Ui
{
class Border
	: public ParentElem<EmptyChildOptions>
{
public:
	Border();
	const std::string GetTypeName() const override { return "Border"; }

	void SetBorderColor(const Float3& borderColor);
	void SetBackgroundColor(const Float3& backgroundColor);

	static constexpr float BORDER_THICKNESS{ UiSystem::BORDER_THICKNESS };
	static constexpr float MARGIN_THICKNESS{ BORDER_THICKNESS * 2 };

private:
	void TreeUpdate(const ResizePref& pref) override;
	void Clear() override;
	void Create() override;

	RectId m_BorderId{};
	RectId m_BackgroundId{};
	Float3 m_BorderColor;
	Float3 m_BackgroundColor;
};
}
}
