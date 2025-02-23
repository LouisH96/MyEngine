#pragma once
#include "..\EmptyChildOptions.h"
#include "..\ParentElem.h"

namespace MyEngine
{
namespace Ui
{
class Margin final : public ParentElem<EmptyChildOptions>
{
public:
	explicit Margin(float margin);
	const std::string GetTypeName() const override { return "Margin"; }

private:
	void UpdateSizeAndTreePositions(const ResizePref& pref) override;
	void Clear() override;
	void Create() override;

	float m_Margin;
};
}
}
