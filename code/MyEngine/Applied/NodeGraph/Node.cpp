#include "pch.h"
#include "Node.h"

#include "DataStructures/Adders/IncrementRefAdder.h"

using namespace Applied;

const unsigned Node::NR_INDICES = Generator::GetNrIndices() * NR_RECTS;
const unsigned Node::NR_VERTICES = Generator::GetNrVertices() * NR_RECTS;

Node::Node()
{
}

Node::Node(const RectFloat& rect, const Float3& color)
	: m_FullRect{ rect }
	, m_Color{ color }
{
	UpdatePartialRects();
}

void Node::SetHeaderLeftBot(const Float2& leftBot)
{
	const Float2 displacement{ leftBot - m_HeaderRect.GetLeftBot() };
	m_HeaderRect.SetLeftBot(leftBot);

	m_FullRect.Move(displacement);
}

void Node::WriteVertices(Vertex*& pVertices) const
{
	//background
	RectFloat rect{ m_FullRect };
	WriteVertices(pVertices, rect, Float3{ .4f });

	//body
	rect.SetLeft(m_FullRect.GetLeft() + BORDER);
	rect.SetBottom(m_FullRect.GetBottom() + BORDER);
	rect.SetHeight(m_FullRect.GetHeight() - HEADER_HEIGHT - BORDER * 3);
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

void Node::UpdatePartialRects()
{
	m_HeaderRect = RectFloat{
		{m_FullRect.GetLeft() + BORDER, m_FullRect.GetTop() - HEADER_HEIGHT - BORDER},
		{m_FullRect.GetWidth() - BORDER * 2, HEADER_HEIGHT}
	};
}

void Node::WriteVertices(Vertex*& pVertices, const RectFloat& rect, const Float3& color)
{
	Generator::GenerateVertices([&color](const Float2& position)
		{
			return Vertex{ position, color };
		}, IncrementRefAdder<Vertex>(pVertices), rect);
}
