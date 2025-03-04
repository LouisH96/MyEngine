#include "pch.h"
#include "AnchorParent.h"

using namespace Ui;

AnchorParent::AnchorParent()
	: ParentElem{ false }
{
}

void AnchorParent::AddChild(Elem* pElem, const Float2& anchor)
{
	ParentElem::AddChild({ pElem, anchor, anchor, FillMode::Min, FillMode::Min });
}

void AnchorParent::AddChild(Elem* pElem, const Float2& parentAnchor, const Float2& childAnchor)
{
	ParentElem::AddChild({ pElem, parentAnchor, childAnchor, FillMode::Min, FillMode::Min });
}

void AnchorParent::TreeUpdate(const ResizePref& pref)
{
	SetSize(pref);

	//pref.minSize not taking into account (for now)
	ResizePref childPref;
	childPref.minSize = {};

	//Child Sizes
	for (unsigned iChild = 0; iChild < GetNrChildren(); iChild++)
	{
		const AnchorChild& child{ GetChildData(iChild) };
		childPref.horMode = child.FillHor;
		childPref.verMode = child.FillVer;
		childPref.maxSize = pref.maxSize;

		//Width
		if (child.ParentAnchor.x < child.ChildAnchor.x)
			childPref.maxSize.x *= child.ParentAnchor.x / child.ChildAnchor.x;
		else if(child.ParentAnchor.x > child.ChildAnchor.x)
			childPref.maxSize.x *=
			(1 - child.ParentAnchor.x) /
			(1 - child.ChildAnchor.x);

		//Height
		if (child.ParentAnchor.y < child.ChildAnchor.y)
			childPref.maxSize.y *= child.ParentAnchor.y / child.ChildAnchor.y;
		else if(child.ParentAnchor.y > child.ChildAnchor.y)
			childPref.maxSize.y *=
			(1 - child.ParentAnchor.y) /
			(1 - child.ChildAnchor.y);

		ChildUpdateSizeAndTreePositions(iChild, childPref);

		//Update my size
			const Float2& childSize{ GetChild(iChild).GetSize() };
			SetWidth(Float::Max(GetWidth(), childSize.x));
			SetHeight(Float::Max(GetHeight(), childSize.y));
	}

	//Position childs
	for (unsigned iChild{ 0 }; iChild < GetNrChildren(); ++iChild)
	{
		const AnchorChild& child{ GetChildData(iChild) };
		const Float2& childSize{ GetChild(iChild).GetSize() };

		Float2 pos{};
		pos.x = child.ParentAnchor.x * GetWidth();
		pos.x -= child.ChildAnchor.x * childSize.x;

		pos.y = child.ParentAnchor.y * GetHeight();
		pos.y -= child.ChildAnchor.y * childSize.y;
		ChildSetPosition(iChild, pos);
	}
}

void AnchorParent::Clear()
{
}

void AnchorParent::Create()
{
}

void AnchorChild::SetAnchors(const Float2& anchor)
{
	ParentAnchor = anchor;
	ChildAnchor = anchor;
}

void AnchorChild::SetFillMin()
{
	SetFill(FillMode::Min);
}

void AnchorChild::SetFillMax()
{
	SetFill(FillMode::Max);
}

void AnchorChild::SetFill(FillMode fillMode)
{
	FillHor = fillMode;
	FillVer = fillMode;
}

Float2 AnchorChild::GetChildPosition(const Float2& parentSize) const
{
	return parentSize * ParentAnchor - pChild->GetSize() * ChildAnchor;
}
