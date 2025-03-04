#include "pch.h"
#include "ListElem.h"

#include "..\UiSystem.h"

using namespace Ui;

//#define MY_DEBUG

ListElem::ListElem(const Settings& settings)
	:m_ChildMargin{ settings.ChildMargin }
	, m_UniformChildWidth{ settings.UniformChildWidth }
{
}

void ListElem::AddChild(Elem* pChild)
{
	ParentElem::AddChild({ pChild });
}

void ListElem::TreeUpdate(const ResizePref& pref)
{
	ResizePref childPref;
	childPref.maxSize = pref.maxSize;
	childPref.minSize = {};
	childPref.SetMin();

	//---| Set/Find child sizes |---
	Float2 childBounds{ 0, -m_ChildMargin };
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		Elem& child{ GetChild(i) };
		ChildUpdateSizeAndTreePositions(i, childPref);

		const float childWidth{ child.GetWidth() };
#ifdef MY_DEBUG
		if (childWidth > pref.maxSize.x) //warning should come from button. Child shouldn't be able to be too big here.
			Logger::Warning("[ListElem::UpdateSizeAndTreePositions] child's width is wider than list's max width");
#endif

		childBounds.x = Float::Max(childBounds.x, childWidth);
		childBounds.y += child.GetHeight() + m_ChildMargin;

		childPref.maxSize.y -= child.GetHeight() + m_ChildMargin;
	}
	if (childBounds.y < 0) childBounds.y = 0;

	//---| Set list size |---
	Float2 listSize{ childBounds };
	listSize.x = Float::Max(pref.minSize.x, listSize.x);
	listSize.y = Float::Max(pref.minSize.y, listSize.y);

#ifdef MY_DEBUG
	if (listSize.y > pref.maxSize.y) //should be handled earlier
		Logger::Warning("[ListElem::UpdateSizeAndTreePositions] list's height is higher than it's max");
#endif

	if (pref.horMode == FillMode::Max)
		listSize.x = pref.maxSize.x;
	if (pref.verMode == FillMode::Max)
		listSize.y = pref.maxSize.y;

	SetSize(listSize);

	//---| set child positions & same widths if needed |---
	childPref.maxSize = pref.maxSize;
	if (m_UniformChildWidth)
	{
		childPref.horMode = FillMode::Max;
		childPref.maxSize.x = childBounds.x;

		Float2 childPos{ (GetSize() - childBounds) / 2 };
		childPos.y = GetHeight() - childPos.y;

		for (unsigned i = 0; i < GetNrChildren(); i++)
		{
			Elem& child{ GetChild(i) };
			ChildUpdateSizeAndTreePositions(i, childPref);

			childPos.x = (GetWidth() - child.GetWidth()) / 2;
			childPos.y -= child.GetHeight();

			ChildSetPosition(i, childPos);
			childPos.y -= m_ChildMargin;
			childPref.maxSize.y -= child.GetHeight() + m_ChildMargin;
		}
	}
	else
	{
		Float2 childPos{ };
		for (unsigned i = GetNrChildren() - 1; i + 1 != 0; i--)
		{
			Elem& child{ GetChild(i) };

			childPos.x = (GetWidth() - child.GetWidth()) / 2.f;
			ChildSetPosition(i, childPos);
			childPos.y += child.GetHeight() + m_ChildMargin;
		}
	}
}

const std::string ListElem::GetTypeName() const
{
	return "List";
}

void ListElem::Clear()
{
}

void ListElem::Create()
{
}

#undef MY_DEBUG