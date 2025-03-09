#include "pch.h"
#include "ListPanel.h"
#include <String\Convert.h>

using namespace MyEngine;
using namespace Ui;

ListPanel::ListPanel(Direction direction, float childMargin, float lineMargin)
	: ListPanel{ ToFlowDirection(direction), ToFillDirection(direction), childMargin, lineMargin }
{
}

ListPanel::ListPanel(const Float2& flowDir, const Float2& fillDir, float childMargin, float lineMargin)
	: m_FlowDir{ flowDir }
	, m_FillDir{ fillDir }
	, m_ChildMargin{ childMargin }
	, m_LineMargin{ lineMargin }
{
}

void ListPanel::AddChild(Elem* pChild)
{
	ParentElem::AddChild({ pChild });
}

void ListPanel::SetUniformFillSize(bool uniformFillSize)
{
	m_UniformFillSize = uniformFillSize;
}

void ListPanel::TreeUpdate(const ResizePref& pref)
{
	m_NrVisibleChilds = 0;

	const float maxListFlowSize{ m_FlowDir.Abs().Dot(pref.maxSize) };
	const float maxListFillSize{ m_FillDir.Abs().Dot(pref.maxSize) };

	float listFlowSize{ 0 };
	float listFillSize{ 0 };

	unsigned iChild{ 0 };
	bool isLastLine{};

	//
	do
	{
		const unsigned iChildBegin{ iChild };

		float lineFlowSize{ maxListFlowSize }; //now it means the max line flow size, after function it is the actual size
		float lineFillSize{ maxListFillSize - listFillSize }; //not it means the max line fill size, after function it is the actual size

		CreateLineOnOrigin(iChild, lineFlowSize, lineFillSize, isLastLine);

		//extend
		if (m_UniformFillSize)
		{
			ResizePref childPref;
			if (m_FillDir.x != 0)
			{
				childPref.horMode = FillMode::Max;
				childPref.verMode = FillMode::Min;
			}
			else
			{
				childPref.horMode = FillMode::Min;
				childPref.verMode = FillMode::Max;
			}

			for (unsigned iExtend{ iChildBegin }; iExtend < iChild; ++iExtend)
			{
				Elem& child{ GetChild(iExtend) };
				childPref.maxSize = m_FlowDir * m_FlowDir.Dot(child.GetSize());
				childPref.maxSize += m_FillDir.Abs() * lineFillSize;
				ChildTreeUpdate(iExtend, childPref);

				Float2 pos{ m_FlowDir * m_FlowDir.Dot(child.GetPosition()) };
				pos -= m_FillDir.Abs() * m_FillDir.Dot(child.GetSize()) * .5f;
				ChildSetPosition(iExtend, pos);
			}
		}

		//move line
		const Float2 linePos{ m_FillDir * (listFillSize + lineFillSize * .5f) };
		for (unsigned iMove = iChildBegin; iMove < iChild; iMove++)
		{
			Elem& child{ GetChild(iMove) };
			ChildSetPosition(iMove, child.GetPosition() + linePos);
		}

		//apply changes
		listFlowSize = Float::Max(listFlowSize, lineFlowSize);
		listFillSize += lineFillSize + m_LineMargin;

	} while (!isLastLine);

	if (listFillSize > 0)
		listFillSize -= m_LineMargin;

	SetSize(m_FlowDir.Abs() * listFlowSize
		+ m_FillDir.Abs() * listFillSize);


	//move all childs to the pos-x & pos-y quadrant
	const Float2 currentCenter{ (m_FlowDir * listFlowSize + m_FillDir * listFillSize) * .5f };
	const Float2 targetCenter{ GetSize() * .5f };
	const Float2 movement{ targetCenter - currentCenter };

	for (iChild = 0; iChild < m_NrVisibleChilds; iChild++)
	{
		Elem& child{ GetChild(iChild) };
		ChildSetPosition(iChild, child.GetPosition() + movement);
	}
}

void ListPanel::Clear()
{
}

void ListPanel::Create()
{
}

void ListPanel::CreateLineOnOrigin(unsigned& iChild, float& lineFlowSize, float& lineFillSize, bool& isLastLine)
{
	isLastLine = true;

	const float maxFillSize{ lineFillSize };
	const float maxFlowSize{ lineFlowSize };

	ResizePref childPref;
	childPref.SetMin();

	lineFlowSize = 0;
	lineFillSize = 0;

	for (; iChild < GetNrChildren(); iChild++)
	{
		childPref.maxSize =
			m_FlowDir.Abs() * (maxFlowSize - lineFlowSize)
			+ m_FillDir.Abs() * maxFillSize;

		//check child size
		Elem& child{ GetChild(iChild) };
		ChildTreeUpdate(iChild, childPref);

		const float childFlowSize{ abs(child.GetSize().Dot(m_FlowDir)) };
		const float childFillSize{ abs(child.GetSize().Dot(m_FillDir)) };

		const float futureLineFillSize{ Float::Max(lineFillSize, childFillSize) };
		const float futureLineFlowSize{ lineFlowSize + childFlowSize };

		if (futureLineFillSize > maxFillSize)
		{
			//line too big in fill size
			//this is the end of adding childs to this list
			//(because this child's fill size will never fit)

			break;
		}

		if (futureLineFlowSize > maxFlowSize)
		{
			//line too big in flow size
			//this is the end of this line

			isLastLine = childFlowSize > maxFlowSize;
			break;
		}

		//place child on origin axis
		Float2 childPos{ m_FlowDir * (lineFlowSize + childFlowSize * .5f) }; //child center
		childPos -= child.GetSize() * .5f; //child origin

		ChildSetPosition(iChild, childPos);

		//apply changes
		lineFillSize = futureLineFillSize;
		lineFlowSize = futureLineFlowSize + m_ChildMargin;
		m_NrVisibleChilds++;
	}

	if (lineFlowSize > 0)
		lineFlowSize -= m_ChildMargin;
	else if (lineFlowSize < 0)
		lineFlowSize = 0;
}

Float2 ListPanel::ToFlowDirection(Direction direction)
{
	const int d{ static_cast<int>(direction) };

	const int vertical{ (d >> 2) & 1 };
	const int negative{ 1 - ((d >> 1) & 1) * 2 };

	Float2 flow{};
	flow.x = static_cast<float>((~vertical) & 1);
	flow.y = static_cast<float>(vertical & 1);
	flow *= static_cast<float>(negative);

	return flow;
}

Float2 ListPanel::ToFillDirection(Direction direction)
{
	const int d{ static_cast<int>(direction) };

	const int vertical{ (~d >> 2) & 1 };
	const int negative{ 1 - (d & 1) * 2 };

	Float2 fill{};
	fill.x = static_cast<float>((~vertical) & 1);
	fill.y = static_cast<float>(vertical & 1);
	fill *= static_cast<float>(negative);

	return fill;
}
