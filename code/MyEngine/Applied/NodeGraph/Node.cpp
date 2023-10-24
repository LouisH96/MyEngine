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
	: m_Rect{ rect }
	, m_Color{ color }
{
}

void Node::WriteVertices(Vertex*& pVertices) const
{
	RectFloat rect{ m_Rect };

	Generator::GenerateVertices([](const Float2& position)
		{
			return Vertex{ position, Float3{.4f} };
		}, IncrementRefAdder<Vertex>(pVertices), rect);

	rect.SetLeft(rect.GetLeft() + BORDER);
	rect.SetWidth(rect.GetWidth() - BORDER * 2);
	rect.SetBottom(rect.GetTop() - HEADER_HEIGHT - BORDER);
	rect.SetHeight(HEADER_HEIGHT);
	Generator::GenerateVertices([](const Float2& position)
		{
			return Vertex{ position, Float3{.1f} };
		}, IncrementRefAdder<Vertex>(pVertices), rect);

	rect.SetBottom(m_Rect.GetBottom() + BORDER);
	rect.SetHeight(m_Rect.GetHeight() - HEADER_HEIGHT - BORDER * 3);
	Generator::GenerateVertices([this](const Float2& position)
		{
			return Vertex{ position, m_Color };
		}, IncrementRefAdder<Vertex>(pVertices), rect);
}

void Node::WriteIndices(int*& pIndices, unsigned offset)
{
	Generator::GenerateIndices(IncrementRefAdder<int>(pIndices), offset);
	Generator::GenerateIndices(IncrementRefAdder<int>(pIndices), offset += Generator::GetNrVertices());
	Generator::GenerateIndices(IncrementRefAdder<int>(pIndices), offset += Generator::GetNrVertices());
}
