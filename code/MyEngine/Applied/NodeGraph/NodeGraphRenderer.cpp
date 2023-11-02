#include "pch.h"
#include "NodeGraphRenderer.h"

#include "DataStructures/Adders/IncrementRefAdder.h"
#include "Framework/Resources.h"

using namespace Applied;
using namespace Rendering;

NodeGraphRenderer::NodeGraphRenderer()
	: m_InputLayout{ InputLayout::FromType<Node::Vertex>() }
	, m_Shader{ Resources::GlobalShader(L"NodeGraph.hlsl") }
{
}

void NodeGraphRenderer::UpdateData(PtrRangeConst<Node> nodes)
{
	int* pIndices;
	Node::Vertex* pVertices;
	if (m_NrVertices > m_Vertices.GetCapacity())
	{
		//new
		const unsigned newIndicesCapacity = m_NrIndices * 2;
		const unsigned newVerticesCapacity = m_NrVertices * 2;

		pIndices = new int[newIndicesCapacity];
		pVertices = new Node::Vertex[newVerticesCapacity];
		WriteData(nodes, pIndices, pVertices);
		m_Indices = IdxBuffer{ pIndices, newIndicesCapacity, true };
		m_Vertices = Buffer<Node::Vertex>{ {pVertices, newVerticesCapacity}, true };
		delete[] pIndices;
		delete[] pVertices;
	}
	else
	{
		//update
		pIndices = m_Indices.BeginCopyData();
		pVertices = m_Vertices.StartCopyData();
		WriteData(nodes, pIndices, pVertices);
		m_Indices.EndCopyData();
		m_Vertices.EndCopyData();
	}
}

void NodeGraphRenderer::Render(const Camera2D& camera)
{
	m_CameraBuffer.Update(camera.GetCameraBuffer());
	m_CameraBuffer.Activate(0);

	PrimitiveTopology::Activate(Node::TOPOLOGY);
	m_InputLayout.Activate();
	m_Shader.Activate();

	m_Vertices.Activate(0);
	m_Indices.Activate();
	m_Indices.Draw(m_NrIndices);
}

void NodeGraphRenderer::IncreaseNrIndices(int amount)
{
	m_NrIndices += amount;
}

void NodeGraphRenderer::IncreaseNrVertices(int amount)
{
	m_NrVertices += amount;
}

void NodeGraphRenderer::WriteData(PtrRangeConst<Node> nodes, int* pIndices, Node::Vertex* pVertices)
{
	const Node* pNodesEnd{ nodes.End() };
	const Node::Vertex* pVerticesFirst{ pVertices };

	for (const Node* pNode{ &nodes.First() }; pNode != pNodesEnd; pNode++)
	{
		if (!pNode->HasParent())
			continue;
		WriteConnectionIndices(pIndices, static_cast<unsigned>(pVertices - pVerticesFirst));
		WriteConnectionVertices(*pNode, nodes[pNode->GetParentId()], pVertices);
	}

	for (const Node* pNode{ &nodes.First() }; pNode != pNodesEnd; pNode++)
	{
		WriteRectIndices(*pNode, pIndices, static_cast<unsigned>(pVertices - pVerticesFirst));
		WriteRectVertices(*pNode, pVertices);
	}
}

void NodeGraphRenderer::WriteRectVertices(const Node& node, Node::Vertex*& pTarget)
{
	const RectFloat& fullRect{ node.GetFullRect() };

	//background
	RectFloat rect{ fullRect };
	WriteRectVertices(pTarget, rect, Float3{ .4f });

	//body
	rect.SetLeft(fullRect.GetLeft() + Node::BORDER);
	rect.SetBottom(fullRect.GetBottom() + Node::BORDER);
	rect.SetHeight(fullRect.GetHeight() - Node::HeaderHeight - Node::BORDER * 3);
	rect.SetWidth(fullRect.GetWidth() - Node::BORDER * 2);
	WriteRectVertices(pTarget, rect, node.GetColor());

	//header
	WriteRectVertices(pTarget, node.GetHeaderRect(), Float3{ .1f });
}

void NodeGraphRenderer::WriteRectIndices(const Node& node, int*& pTarget, unsigned offset)
{
	for (unsigned i = 0; i < Node::NR_RECTS; i++)
	{
		Node::Generator::GenerateIndices(IncrementRefAdder<int>(pTarget), offset);
		offset += Node::Generator::GetNrVertices();
	}
}

void NodeGraphRenderer::WriteConnectionVertices(const Node& node, const Node& parentNode, Node::Vertex*& pTarget)
{
	constexpr float halfThickness{ Node::BORDER * .5f };
	const Float3 color{ 1,1,0 };

	const Float2 center{ node.GetFullRect().GetCenter() };
	const Float2 parentCenter{ parentNode.GetFullRect().GetCenter() };

	const Float2 toParent{ parentCenter - center };
	const Float2 toParentDir{ toParent.Normalized() };
	const Float2 halfRight{ toParentDir.y * halfThickness, -toParentDir.x * halfThickness };

	const Float2 leftBot{ center - halfRight };
	const Float2 rightBot{ center + halfRight };
	const Float2 leftTop{ parentCenter - halfRight };
	const Float2 rightTop{ parentCenter + halfRight };

	*pTarget++ = Node::Vertex{ leftBot, color };
	*pTarget++ = Node::Vertex{ leftTop, color };
	*pTarget++ = Node::Vertex{ rightTop, color };
	*pTarget++ = Node::Vertex{ rightBot, color };
}

void NodeGraphRenderer::WriteConnectionIndices(int*& pTarget, unsigned offset)
{
	Node::Generator::GenerateIndices(IncrementRefAdder<int>(pTarget), offset);
}

void NodeGraphRenderer::WriteRectVertices(Node::Vertex*& pTarget, const RectFloat& rect, const Float3& color)
{
	Node::Generator::GenerateVertices([&color](const Float2& position)
		{
			return Node::Vertex{ position, color };
		}, IncrementRefAdder<Node::Vertex>(pTarget), rect);
}
