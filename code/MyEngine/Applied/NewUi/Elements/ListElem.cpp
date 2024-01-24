#include "pch.h"
#include "ListElem.h"

using namespace NewUi;

ListElem::ListElem(const Float2& borderMargin, float childMargin)
	: m_BorderMargin{ borderMargin }
	, m_ChildMargin{ childMargin }
{
}

void ListElem::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	ResizePref childPref;
	childPref.maxSize = pref.maxSize;
	childPref.minSize = {};
	childPref.horMode = Min;
	childPref.verMode = Min;

	float widest{ 0 };
	float height{ 0 };
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		Elem& child{ GetChild(i) };
		UpdateChildSize(i, childPref);

		const float childWidth{ child.GetWidth() };
		if (childWidth > pref.maxSize.x)
			Logger::PrintError("[ListElem::PreResize] child's width is wider than list's max width");

		if (childWidth > widest) widest = childWidth;

		height += child.GetHeight() + m_ChildMargin;
	}
	if (height > 0) height -= m_ChildMargin;

	//set list size
	if (widest < pref.minSize.x)
		widest = pref.minSize.x;
	if (height > pref.maxSize.y)
		Logger::PrintError("[ListElem::PreResize] list's height is higher than it's max");
	if (height < pref.minSize.y)
		height = pref.minSize.y;

	SetSize({ widest, height });

	//set child positions
	Float2 childPos{};
	for (unsigned i = GetNrChildren() - 1; i + 1 != 0; i--)
	{
		Elem& child{ GetChild(i) };

		childPos.x = (GetWidth() - child.GetWidth()) / 2.f;
		SetChildPosition(i, childPos);
		childPos.y += child.GetHeight() + m_ChildMargin;
	}
}

void ListElem::Clear()
{}

void ListElem::Create()
{}
