#include "pch.h"
#include "RelativeParent.h"

using namespace NewUi;

RelativeParent::RelativeParent()
	: ParentElem{ false }
{
}

void RelativeParent::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	SetSize(pref.GetPreferredSize());

	ResizePref resizePref;
	resizePref.minSize = {};
	resizePref.horMode = Max;
	resizePref.verMode = Max;

	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		const RelativeChild& child{ GetChildData(i) };

		resizePref.maxSize = GetSize() * Float2 { child.MaxWidthRatio, child.MaxHeightRatio };
		UpdateChildSize(i, resizePref);

		Float2 childPos{ (GetSize() - child.pChild->GetSize()) / 2.f };
		SetChildPosition(i, childPos);
	}
}

const std::string RelativeParent::GetTypeName() const
{
	return "RelativeParent";
}

void RelativeParent::Clear()
{}

void RelativeParent::Create()
{}
