#include "pch.h"
#include "NodeGraph.h"

#include "GraphSorter.h"
#include "Gui/FontRenderer.h"

using namespace Applied;

NodeGraph::NodeGraph()
{
	//
	Node::HeaderHeight = m_FontRenderer.GetMaxTextHeight() + Node::BORDER * 2;

	//
	m_CameraController.SetCamera(&m_Camera);
	m_CameraController.SetMoveSpeed(.002f);
	m_CameraController.SetZoom(.15f);

	const int n00 = Add({ 0,0 }, 1, { 0,0,.7f }, "00");

	const int n10 = Add({ 0,0 }, 1, { 0,0,.7f }, "10");
	const int n11 = Add({ 0,0 }, 1, { 0,0,.7f }, "11");
	const int n12 = Add({ 0,0 }, 1, { 0,0,.7f }, "12");

	const int n20 = Add({ 0,0 }, 1, { 0,0,.7f }, "20");
	const int n21 = Add({ 0,0 }, 1, { 0,0,.7f }, "21");
	const int n22 = Add({ 0,0 }, 1, { 0,0,.7f }, "22");
	const int n23 = Add({ 0,0 }, 1, { 0,0,.7f }, "23");
	const int n24 = Add({ 0,0 }, 1, { 0,0,.7f }, "24");

	SetParent(n10, n00);
	SetParent(n11, n00);
	SetParent(n12, n00);

	SetParent(n20, n10);

	SetParent(n21, n12);
	SetParent(n22, n12);
	SetParent(n23, n12);
	SetParent(n24, n12);

	GraphSorter::Sort(*this);
}

void NodeGraph::Update()
{
	const NodeGrabbing::NodeMoved movedEvent{ m_NodeGrabbing.Update(m_Nodes, m_Camera) };
	if (movedEvent.HasMoved())
	{
		Node& node{ m_Nodes.Get(movedEvent.NodeId) };
		node.SetHeaderLeftBot(node.GetHeaderLeftBot() + movedEvent.Displacement);
		m_FontRenderer.Move(node.GetHeaderTextId(), movedEvent.Displacement);
	}

	m_CameraController.EarlyUpdate();
	m_Camera.Update();

	if (m_Nodes.HasChanged())
	{
		m_Renderer.UpdateData({ m_Nodes.GetFirst(), m_Nodes.GetSize() });
		m_Nodes.ClearChangedFlag();
	}

	m_FontRenderer.Update();
}

void NodeGraph::Render()
{
	m_Renderer.Render(m_Camera);
	m_FontRenderer.Render();
}

void NodeGraph::OnCanvasResized(const App::ResizedEvent& event)
{
	m_Camera.OnCanvasResized(event);
}

int NodeGraph::Add(const Float2& leftTop, float contentHeight, const Float3& color, const std::string& text)
{
	m_Renderer.IncreaseNrIndices(Node::NR_INDICES);
	m_Renderer.IncreaseNrVertices(Node::NR_VERTICES);

	float baseline;
	const Float2 textSize{ m_FontRenderer.GetTextSize(text, NodeGraphFontRenderer::HEADER_FONT_SIZE, baseline) };

	const Float2 nodeSize{
		Float::Max(textSize.x, .5f) + Node::BORDER * 8,
		Node::HeaderHeight + Node::BORDER * 2 + (contentHeight > 0 ? contentHeight + Node::BORDER : 0)
	};

	const RectFloat nodeRect{ leftTop - Float2{0, nodeSize.y}, nodeSize };

	Node node{ nodeRect, color, text };

	node.SetHeaderTextId(m_FontRenderer.Add(
		NodeGraphFontRenderer::TextInfo{
			node.GetHeaderText(), NodeGraphFontRenderer::HEADER_FONT_SIZE, Float3{ .4f }
		},
		NodeGraphFontRenderer::PositionInfo{
			node.GetHeaderRect().GetCenter(),{.5f,.5f}
		}, textSize, baseline));

	return m_Nodes.Add(node);
}

void NodeGraph::SetParent(unsigned childId, unsigned parentId)
{
	m_Nodes.Get(childId).SetParentNode(parentId);
	if (parentId != Node::INVALID_ID)
	{
		m_Renderer.IncreaseNrIndices(Node::NR_CONNECTION_INDICES);
		m_Renderer.IncreaseNrVertices(Node::NR_CONNECTION_VERTICES);
	}
}

void NodeGraph::SetPos(unsigned nodeIdx, Float2 position, const Float2& pivot)
{
	Node& node{ m_Nodes.Get(nodeIdx) };

	position -= pivot * node.GetFullRect().GetSize();
	const Float2 displacement{ position - node.GetFullRect().GetLeftBot() };
	node.SetLeftBot(position);
	m_FontRenderer.Move(node.GetHeaderTextId(), displacement);
}

const Node& NodeGraph::GetNode(unsigned nodeIdx) const
{
	return m_Nodes.Get(nodeIdx);
}

unsigned NodeGraph::GetDepth(unsigned nodeIdx) const
{
	unsigned depth{ 0 };

	const Node* pNode{ &m_Nodes.Get(nodeIdx) };
	while (pNode->HasParent())
	{
		depth++;
		pNode = &m_Nodes.Get(pNode->GetParentId());
	}

	return depth;
}

unsigned NodeGraph::GetRootNodeId(unsigned nodeId) const
{
	const Node* pNode{ &m_Nodes.Get(nodeId) };
	if (!pNode->HasParent())
		return nodeId;

	while (pNode->HasParent() && m_Nodes.Get(pNode->GetParentId()).HasParent())
		pNode = &m_Nodes.Get(pNode->GetParentId());

	return pNode->GetParentId();
}
