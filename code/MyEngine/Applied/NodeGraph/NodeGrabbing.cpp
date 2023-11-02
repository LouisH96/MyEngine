#include "pch.h"
#include "NodeGrabbing.h"

#include "Applied/2D/Camera2D.h"

using namespace Applied;

NodeGrabbing::NodeMoved NodeGrabbing::Update(const InvalidateList<Node>& nodes, const Camera2D& camera)
{
	NodeMoved movedEvent{};

	if (IsGrabbing())
	{
		if (MOUSE.IsLeftBtnDown())
		{
			//move
			const Float2 mousePos{ camera.GetMouseWorldPos() };
			const Node& grabbed{ GetGrabbed(nodes) };
			const Float2 newPos{ mousePos - m_GrabOffset };

			movedEvent.NodeId = m_Grabbed;
			movedEvent.Displacement = newPos - grabbed.GetHeaderLeftBot();
		}
		else
		{
			//release
			m_Grabbed = Node::INVALID_ID;
		}
	}
	else if (MOUSE.IsLeftBtnPressed())
	{
		m_Grabbed = GetNodeUnderMouse(nodes, camera);
		if (m_Grabbed != Node::INVALID_ID)
		{
			const Node& grabbed{ GetGrabbed(nodes) };
			const RectFloat& rect{ grabbed.GetHeaderRect() };
			const Float2 mousePos{ camera.GetMouseWorldPos() };
			m_GrabOffset = mousePos - rect.GetLeftBot();
		}
	}

	return movedEvent;
}

const Node& NodeGrabbing::GetGrabbed(const InvalidateList<Node>& nodes) const
{
	return nodes.Get(m_Grabbed);
}

unsigned NodeGrabbing::GetNodeUnderMouse(const InvalidateList<Node>& nodes, const Camera2D& camera)
{
	const Float2 mousePos{ camera.GetMouseWorldPos() };

	for (int i = nodes.GetFirstIdx(); i < nodes.GetEndIdx(); i++)
	{
		const Node& node{ nodes.Get(i) };
		if (!node.IsValid())
			continue;

		if (node.GetHeaderRect().ContainsPoint(mousePos))
			return i;
	}

	return Node::INVALID_ID;
}
