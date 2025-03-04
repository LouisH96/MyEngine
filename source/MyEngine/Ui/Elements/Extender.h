#pragma once
#include "..\EmptyChildOptions.h"
#include "..\ParentElem.h"
#include "..\SizeDef.h"

namespace MyEngine
{
namespace Ui
{
class Extender final
	: public ParentElem<EmptyChildOptions>
{
public:
	explicit Extender(const SizeDef& sizeDef);
	const std::string GetTypeName() const override { return "Extender"; }
	void TreeUpdate(const ResizePref& pref) override;

	void SetSizeDef(const Float2& value);
	void SetSizeDefX(float valueX);
	void SetSizeDefY(float valueY);

private:
	void Clear() override;
	void Create() override;

	SizeDef m_SizeDef;
};
}
}
