#include "pch.h"
#include "Node.h"

#include "DataStructures/Adders/IncrementRefAdder.h"

using namespace Applied;

const unsigned Node::NR_INDICES = Generator::GetNrIndices() * NR_RECTS;
const unsigned Node::NR_VERTICES = Generator::GetNrVertices() * NR_RECTS;
float Node::HeaderHeight = .5f;

Node::Node()
{
}

Node::Node(const RectFloat& rect, const Float3& color, const std::string& headerText)
	: m_FullRect{ rect }
	, m_Color{ color }
	, m_HeaderText{ headerText }
{
	UpdatePartialRects();
}

void Node::SetHeaderLeftBot(const Float2& leftBot)
{
	const Float2 displacement{ leftBot - m_HeaderRect.GetLeftBot() };
	m_HeaderRect.SetLeftBot(leftBot);

	m_FullRect.Move(displacement);
}

void Node::SetParentNode(unsigned parentId)
{
	m_ParentNode = parentId;
}

bool Node::HasParent() const
{
	return m_ParentNode != INVALID_ID;
}

void Node::WriteVertices(Vertex*& pVertices) const
{
	//background
	RectFloat rect{ m_FullRect };
	WriteVertices(pVertices, rect, Float3{ .4f });

	//body
	rect.SetLeft(m_FullRect.GetLeft() + BORDER);
	rect.SetBottom(m_FullRect.GetBottom() + BORDER);
	rect.SetHeight(m_FullRect.GetHeight() - HeaderHeight - BORDER * 3);
	rect.SetWidth(m_FullRect.GetWidth() - BORDER * 2);
	WriteVertices(pVertices, rect, m_Color);

	//header
	WriteVertices(pVertices, m_HeaderRect, Float3{ .1f });
}

void Node::WriteIndices(int*& pIndices, unsigned offset)
{
	for (unsigned i = 0; i < NR_RECTS; i++)
	{
		Generator::GenerateIndices(IncrementRefAdder<int>(pIndices), offset);
		offset += Generator::GetNrVertices();
	}
}

void Node::WriteConnectionVertices(Vertex*& pVertices, const Node* pNodes) const
{
	if (m_ParentNode == INVALID_ID)
		return;

	constexpr float halfThickness{ BORDER * .5f };
	const Float3 color{ 1,1,0 };

	const Node& parent{ pNodes[m_ParentNode] };
	const Float2 center{ m_FullRect.GetCenter() };
	const Float2 parentCenter{ parent.GetFullRect().GetCenter() };

	const Float2 toParent{ parentCenter - center };
	const Float2 toParentDir{ toParent.Normalized() };
	const Float2 halfRight{ toParentDir.y * halfThickness, -toParentDir.x * halfThickness };

	const Float2 leftBot{ center - halfRight };
	const Float2 rightBot{ center + halfRight };
	const Float2 leftTop{ parentCenter - halfRight };
	const Float2 rightTop{ parentCenter + halfRight };

	*pVertices++ = Vertex{ leftBot, color };
	*pVertices++ = Vertex{ leftTop, color };
	*pVertices++ = Vertex{ rightTop, color };
	*pVertices++ = Vertex{ rightBot, color };
}

void Node::WriteConnectionIndices(int*& pIndices, unsigned offset) const
{
	if (m_ParentNode == INVALID_ID)
		return;
	Generator::GenerateIndices(IncrementRefAdder<int>(pIndices), offset);
}

void Node::UpdatePartialRects()
{
	m_HeaderRect = RectFloat{
		{m_FullRect.GetLeft() + BORDER, m_FullRect.GetTop() - HeaderHeight - BORDER},
		{m_FullRect.GetWidth() - BORDER * 2, HeaderHeight}
	};
}

void Node::WriteVertices(Vertex*& pVertices, const RectFloat& rect, const Float3& color)
{
	Generator::GenerateVertices([&color](const Float2& position)
		{
			return Vertex{ position, color };
		}, IncrementRefAdder<Vertex>(pVertices), rect);
}
