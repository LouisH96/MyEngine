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

	const int first = Add(Node{ RectFloat{{0,0 },{2,2}}, {.7f,0,0}, "Root" });
	const int second = Add(Node{ RectFloat{{3,0},{2,4}},{0,0,.7f}, "Blue" });
	const int third = Add(Node{ RectFloat({6,3},{2,2}),{0,1,0} , "Green" });
	const int fourth = Add(Node{ RectFloat({3,7},{2,3}),{1,1,1}, "White" });

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

int NodeGraph::Add(Node node)
{
	m_Renderer.IncreaseNrIndices(Node::NR_INDICES);
	m_Renderer.IncreaseNrVertices(Node::NR_VERTICES);

	if (node.HasParent())
	{
		m_Renderer.IncreaseNrIndices(Node::NR_CONNECTION_INDICES);
		m_Renderer.IncreaseNrVertices(Node::NR_CONNECTION_VERTICES);
	}

	node.SetHeaderTextId(m_FontRenderer.Add(
		NodeGraphFontRenderer::TextInfo{
			node.GetHeaderText(), NodeGraphFontRenderer::HEADER_FONT_SIZE, Float3{ .4f }
		},
		NodeGraphFontRenderer::PositionInfo{
			node.GetHeaderRect().GetCenter(),{.5f,.5f}
		}));

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
