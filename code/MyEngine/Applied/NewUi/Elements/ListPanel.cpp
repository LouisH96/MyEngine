#include "pch.h"
#include "ListPanel.h"

#include <Applied/NewUi/Renderers/NewUiShapeRenderer.h>

using namespace NewUi;

ListPanel::ListPanel(Direction direction, float childMargin)
	: ListPanel{ ToFlowDirection(direction), ToFillDirection(direction), childMargin }
{
}

ListPanel::ListPanel(const Float2& flowDirection, const Float2& fillDirection, float childMargin)
	: m_FlowDirection{ flowDirection }
	, m_FillDirection{ fillDirection }
	, m_ChildMargin{ childMargin }
{
}

void ListPanel::AddChild(Elem* pChild)
{
	ParentElem::AddChild({ pChild });
}

Float2 ListPanel::ToFlowDirection(Direction direction)
{
	const int vertical{ static_cast<int>(direction) & 1 };
	const int negative{ static_cast<int>(direction) & 2 };

	return Float2{
		static_cast<float>((1 - negative) * (~vertical & 1)),
		static_cast<float>((1 - negative) * vertical)
	};
}

Float2 ListPanel::ToFillDirection(Direction direction)
{
	const int vertical{ static_cast<int>(direction) & 1 };

	return Float2{
		static_cast<float>(vertical),
		static_cast<float>(~vertical & 1)
	};
}

void ListPanel::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	ResizePref childPref;
	childPref.maxSize = pref.maxSize; //will be used as the free space for childs
	childPref.minSize = {};
	childPref.horMode = Min;
	childPref.verMode = Min;

	//---| Size Childs |---
	m_NrVisibleChilds = 0;
	const float maxFillSize{ abs(m_FillDirection.Dot(pref.maxSize)) };
	float fillBounds{ 0 };
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		Elem& child{ GetChild(i) };
		UpdateChildSize(i, childPref);

		Float2 newFreeSpace = childPref.maxSize;

		newFreeSpace -= m_FlowDirection * child.GetSize().Dot(m_FlowDirection);
		newFreeSpace -= Float2{ m_FlowDirection * m_ChildMargin }.Abs();

		if (newFreeSpace.x < 0)
		{
			Logger::PrintWarning("[ListPanel::UpdateSizeAndTreePositions] list not wide enough");
			break;
		}
		if (newFreeSpace.y < 0)
		{
			Logger::PrintWarning("[ListPanel::UpdateSizeAndTreePositions] list not high enough");
			break;
		}

		const float fillSize{ child.GetSize().Dot(m_FillDirection) };
		if (fillSize > maxFillSize)
		{
			Logger::PrintWarning("[ListPanel::UpdateSizeAndTreePositions] Element is too big in Fill-Direction");
			break;
		}

		childPref.maxSize = newFreeSpace;
		fillBounds = Float::Max(fillBounds, fillSize);
		m_NrVisibleChilds++;
	}
	childPref.maxSize += Float2{ m_FlowDirection * m_ChildMargin }.Abs();

	Float2 listSize{
		pref.maxSize - childPref.maxSize
		+ m_FillDirection * fillBounds };
	listSize.x = Float::Max(0, listSize.x);
	listSize.y = Float::Max(0, listSize.y);
	SetSize(listSize);

	//---| Position Childs |---
	Float2 childFlowPos{ listSize / 2.f };
	childFlowPos -= m_FlowDirection * abs(m_FlowDirection.Dot(childFlowPos)); //begin in flowDirection, center in fillDirection

	for (unsigned i = 0; i < m_NrVisibleChilds; i++)
	{
		Elem& child{ GetChild(i) };

		const Float2 childCenter{ child.GetCenter() };
		Float2 childPos{ childFlowPos - childCenter };
		childPos += m_FlowDirection * abs(m_FlowDirection.Dot(childCenter));

		SetChildPosition(i, childPos);

		childFlowPos += m_FlowDirection * (abs(m_FlowDirection.Dot(child.GetSize())) + m_ChildMargin);
	}
}

void ListPanel::Clear()
{
	//Draw().Remove(m_DebugBg);
}

void ListPanel::Create()
{
	//m_DebugBg = Draw().Rect(GetPosition(), GetSize()* Float2{ .5f, 1.f }, { .5f,0,0 });
}
