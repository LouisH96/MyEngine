#include "pch.h"
#include "AnchorParent.h"

using namespace NewUi;

void AnchorParent::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	SetSize(pref);

	ResizePref childPref;
	childPref.minSize = {};
	childPref.horMode = Max;
	childPref.verMode = Max;

	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		const AnchorChild& child{ GetChildData(i) };

		//Width
		const float parentSizeLeft{ GetWidth() * child.ParentAnchor.x };
		const float parentSizeRight{ GetWidth() - parentSizeLeft };

		const float maxLeft{ child.ChildAnchor.x != 0 ? parentSizeLeft / child.ChildAnchor.x
			: Constants::FLOAT_INFINITY };
		const float maxRight{ child.ChildAnchor.x != 1 ? parentSizeRight / (1 - child.ChildAnchor.x)
			: Constants::FLOAT_INFINITY };

		childPref.maxSize.x = Float::Min(maxLeft, maxRight);

		//Height
		const float parentSizeBot{ GetHeight() * child.ParentAnchor.y };
		const float parentSizeTop{ GetHeight() - parentSizeBot };

		const float maxBot{ child.ChildAnchor.y != 0 ? parentSizeBot / child.ChildAnchor.y
			: Constants::FLOAT_INFINITY };
		const float maxTop{ child.ChildAnchor.y != 1 ? parentSizeTop / (1 - child.ChildAnchor.y)
			: Constants::FLOAT_INFINITY };

		childPref.maxSize.y = Float::Min(maxTop, maxBot);

		//
		UpdateChildSize(i, childPref);
		const Float2 childPos
		{
			parentSizeLeft - child.ChildAnchor.x * ChildWidth(i),
			parentSizeBot - child.ChildAnchor.y * ChildHeight(i)
		};
		SetChildPosition(i, childPos);
	}
}

void AnchorParent::Clear()
{
}

void AnchorParent::Create()
{
}
