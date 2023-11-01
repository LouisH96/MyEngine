#include "pch.h"
#include "NodeGraph.h"

#include "Gui/FontRenderer.h"

using namespace Applied;

NodeGraph::NodeGraph()
{
	//
	constexpr float fontSize{ .2f };
	Node::HeaderHeight = FONT.GetMaxHeight(fontSize) + Node::BORDER * 2;

	//
	m_CameraController.SetCamera(&m_Camera);
	m_CameraController.SetMoveSpeed(.002f);
	m_CameraController.SetZoom(.15f);

	const int first = Add(Node{ RectFloat{{0,0 },{2,2}}, {.7f,0,0} });
	const int second = Add(Node{ RectFloat{{3,0},{2,4}},{0,0,.7f} });
	const int third = Add(Node{ RectFloat({6,3},{2,2}),{0,1,0} });
	const int fourth = Add(Node{ RectFloat({3,7},{2,3}),{1,1,1} });

	SetParent(second, first);
	SetParent(third, second);
	SetParent(fourth, first);
}

void NodeGraph::Update()
{
	m_NodeGrabbing.Update(m_Nodes, m_Camera);

	m_CameraController.EarlyUpdate();
	m_Camera.Update();

	if (m_Nodes.HasChanged())
	{
		m_Renderer.UpdateData({ m_Nodes.GetFirst(), m_Nodes.GetSize() });
		m_Nodes.ClearChangedFlag();
	}
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

int NodeGraph::Add(const Node& node)
{
	m_Renderer.IncreaseNrIndices(Node::NR_INDICES);
	m_Renderer.IncreaseNrVertices(Node::NR_VERTICES);

	if (node.HasParent())
	{
		m_Renderer.IncreaseNrIndices(Node::NR_CONNECTION_INDICES);
		m_Renderer.IncreaseNrVertices(Node::NR_CONNECTION_VERTICES);
	}

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
