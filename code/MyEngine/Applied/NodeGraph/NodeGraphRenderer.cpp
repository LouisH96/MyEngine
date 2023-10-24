#include "pch.h"
#include "NodeGraphRenderer.h"

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
	m_NrIndices = Node::NR_INDICES * nodes.count;
	m_NrVertices = Node::NR_VERTICES * nodes.count;

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
	m_Indices.Draw();
}

void NodeGraphRenderer::WriteData(PtrRangeConst<Node> nodes, int* pIndices, Node::Vertex* pVertices)
{
	const Node* pNodesEnd{ nodes.End() };
	const Node::Vertex* pVerticesFirst{ pVertices };

	for (const Node* pNode{ &nodes.First() }; pNode != pNodesEnd; pNode++)
	{
		pNode->WriteIndices(pIndices, static_cast<unsigned>(pVertices - pVerticesFirst));
		pNode->WriteVertices(pVertices);
	}
}
