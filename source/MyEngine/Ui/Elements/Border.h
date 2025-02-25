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

private:
	void UpdateSizeAndTreePositions(const ResizePref& pref) override;
	void Clear() override;
	void Create() override;

	RectId m_BorderId{};
	RectId m_BackgroundId{};
};
}
}
