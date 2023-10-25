#include "pch.h"
#include "NodeGraph.h"

using namespace Applied;

NodeGraph::NodeGraph()
{
	m_CameraController.SetCamera(&m_Camera);
	m_CameraController.SetMoveSpeed(.002f);
	m_CameraController.SetZoom(.15f);

	m_Nodes.Add(Node{ RectFloat{{},{2,2}}, {.7f,0,0} });
	m_Nodes.Add(Node{ RectFloat{{3,3},{2,4}},{0,0,.7f} });
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
}
