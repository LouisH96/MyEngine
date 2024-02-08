#include "pch.h"
#include "ListElem.h"

#include "Applied/NewUi/NewUiSystem.h"
#include "Gui/GuiRenderer.h"

using namespace NewUi;

ListElem::ListElem(const Settings& settings)
	: m_MainMargin{ settings.VisualBorder ? NewUiSystem::BORDER_THICKNESS * 2 : 0 }
	, m_ChildMargin{ settings.ChildMargin }
	, m_UniformChildWidth{ settings.UniformChildWidth }
	, m_BorderId{ settings.VisualBorder ? 0 : GetNoBorderId() }
	, m_BackgroundId{} //don't have to be set
{
}

void ListElem::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	ResizePref childPref;
	childPref.maxSize = pref.maxSize - m_MainMargin * 2;
	childPref.minSize = {};
	childPref.horMode = Min;
	childPref.verMode = Min;

	//---| Set/Find child sizes |---
	Float2 childBounds{ 0, -m_ChildMargin };
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		Elem& child{ GetChild(i) };
		UpdateChildSize(i, childPref);

		const float childWidth{ child.GetWidth() };
		if (childWidth > pref.maxSize.x) //warning should come from button. Child shouldn't be able to be too big here.
			Logger::PrintWarning("[ListElem::UpdateSizeAndTreePositions] child's width is wider than list's max width");

		childBounds.x = Float::Max(childBounds.x, childWidth);
		childBounds.y += child.GetHeight() + m_ChildMargin;
	}
	if (childBounds.y < 0) childBounds.y = 0;

	//---| Set list size |---
	Float2 listSize{ childBounds + m_MainMargin * 2 };
	listSize.x = Float::Max(pref.minSize.x, listSize.x);
	listSize.y = Float::Max(pref.minSize.y, listSize.y);

	if (listSize.y > pref.maxSize.y) //should be handled earlier
		Logger::PrintWarning("[ListElem::UpdateSizeAndTreePositions] list's height is higher than it's max");

	if (pref.horMode == Max)
		listSize = pref.maxSize;

	SetSize(listSize);

	//---| set child positions & same widths if needed |---
	if (m_UniformChildWidth)
	{
		childPref.horMode = Max;
		childPref.maxSize.x = childBounds.x;

		Float2 childPos{ (GetSize() - childBounds) / 2 };

		for (unsigned i = GetNrChildren() - 1; i + 1 != 0; i--)
		{
			Elem& child{ GetChild(i) };
			UpdateChildSize(i, childPref);
			SetChildPosition(i, childPos);
			childPos.y += child.GetHeight() + m_ChildMargin;
		}
	}
	else
	{
		Float2 childPos{ m_MainMargin };
		for (unsigned i = GetNrChildren() - 1; i + 1 != 0; i--)
		{
			Elem& child{ GetChild(i) };

			childPos.x = (GetWidth() - child.GetWidth()) / 2.f;
			SetChildPosition(i, childPos);
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
	if (!UseVisualBorder())
		return;

	GUI.Remove(m_BorderId);
	GUI.Remove(m_BackgroundId);
}

void ListElem::Create()
{
	if (!UseVisualBorder())
		return;

	m_BorderId = GUI.Add({ -1,-1 }, GetPosition(), GetSize(), NewUiSystem::COLOR_MEDIUM);
	m_BackgroundId = GUI.Add({ -1,-1 }, GetPosition() + NewUiSystem::BORDER_THICKNESS, GetSize() - m_MainMargin, NewUiSystem::COLOR_DARK);
}

bool ListElem::UseVisualBorder() const
{
	return m_BorderId != GetNoBorderId();
}

unsigned ListElem::GetNoBorderId()
{
	return Uint::MAX;
}
