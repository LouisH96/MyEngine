#include "pch.h"
#include "NodeGraph.h"

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

	const int first = Add({ 0,0 }, 1, { .7f,0,0 }, "Root");
	const int second = Add({ 3,0 }, 1.5f, { 0,0,.7f }, "Blue");
	const int third = Add({ 6,3 }, 0, { 0,1,0 }, "Green");
	const int fourth = Add({ 3,7 }, 1, { 1,1,1 }, "Long header");

	SetParent(second, first);
	SetParent(third, second);
	SetParent(fourth, first);
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
