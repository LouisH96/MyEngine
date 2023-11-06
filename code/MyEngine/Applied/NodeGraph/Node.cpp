#include "pch.h"
#include "Node.h"

using namespace Applied;

const unsigned Node::NR_INDICES = Generator::GetNrIndices() * NR_RECTS;
const unsigned Node::NR_VERTICES = Generator::GetNrVertices() * NR_RECTS;
float Node::HeaderHeight = .5f;

Node::Node()
{
}

Node::Node(const RectFloat& rect, const Float3& color, const std::string& headerText, const Float3& textBgColor)
	: m_FullRect{ rect }
	, m_Color{ color }
	, m_TextBgColor{ textBgColor }
	, m_HeaderText{ headerText }
{
	UpdatePartialRects();
}

void Node::SetLeftBot(const Float2& leftBot)
{
	const Float2 displacement{ leftBot - m_FullRect.GetLeftBot() };
	m_FullRect.SetLeftBot(leftBot);
	m_HeaderRect.Move(displacement);
}

void Node::SetHeaderLeftBot(const Float2& leftBot)
{
	const Float2 displacement{ leftBot - m_HeaderRect.GetLeftBot() };
	m_HeaderRect.SetLeftBot(leftBot);

	m_FullRect.Move(displacement);
}

void Node::SetHeaderLeftTop(const Float2& leftTop)
{
	SetHeaderLeftBot({ leftTop.x, leftTop.y - m_HeaderRect.GetHeight() });
}

void Node::SetParentNode(unsigned parentId)
{
	m_ParentNode = parentId;
}

bool Node::HasParent() const
{
	return m_ParentNode != INVALID_ID;
}

void Node::UpdatePartialRects()
{
	m_HeaderRect = RectFloat{
		{m_FullRect.GetLeft() + BORDER, m_FullRect.GetTop() - HeaderHeight - BORDER},
		{m_FullRect.GetWidth() - BORDER * 2, HeaderHeight}
	};
}
