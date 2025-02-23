#pragma once
#include "..\ParentElem.h"

namespace MyEngine
{
namespace Ui
{
struct AnchorChild
{
	Elem* pChild{};
	Float2 ParentAnchor;
	Float2 ChildAnchor;

	//relative to parent
	Float2 GetChildPosition(const Float2& parentSize) const;
};

class AnchorParent final
	: public ParentElem<AnchorChild>
{
public:
	AnchorParent();

	using ParentElem::AddChild;
	void AddChild(Elem* pElem, const Float2& anchor);
	void AddChild(Elem* pElem, const Float2& parentAnchor, const Float2& childAnchor);

	void UpdateSizeAndTreePositions(const ResizePref& pref) override;
	const std::string GetTypeName() const override { return "AnchorParent"; }

private:
	void Clear() override;
	void Create() override;
};
}
}
