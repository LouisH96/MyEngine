#include "pch.h"
#include "NodeGraph.h"

using namespace Applied;

NodeGraph::NodeGraph()
{
	m_CameraController.SetCamera(&m_Camera);
	m_CameraController.SetMoveSpeed(.002f);
	m_CameraController.SetZoom(.15f);
}

void NodeGraph::Update()
{
	m_CameraController.EarlyUpdate();
	m_Camera.Update();
}

void NodeGraph::Render()
{
	m_Renderer.Render(m_Camera);
}
