#include "pch.h"
#include "Node.h"

#include "DataStructures/Adders/IncrementRefAdder.h"

using namespace Applied;

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
	Generator::GenerateVertices([this](const Float2& position)
		{
			return Vertex{ position, m_Color };
		}, IncrementRefAdder<Vertex>(pVertices), m_Rect);
}

void Node::WriteIndices(int*& pIndices, unsigned offset)
{
	Generator::GenerateIndices(IncrementRefAdder<int>(pIndices), offset);
}
