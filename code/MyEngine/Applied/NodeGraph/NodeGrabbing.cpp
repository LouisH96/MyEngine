#include "pch.h"
#include "NodeGrabbing.h"

#include "Applied/2D/Camera2D.h"

using namespace Applied;

void NodeGrabbing::Update(InvalidateList<Node>& nodes, const Camera2D& camera)
{
	if (IsGrabbing())
	{
		if (MOUSE.IsLeftBtnDown())
		{
			//move
			const Float2 mousePos{ camera.GetMouseWorldPos() };
			Node& grabbed{ GetGrabbed(nodes) };
			grabbed.SetHeaderLeftBot(mousePos - m_GrabOffset);
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
		if (m_Grabbed == Node::INVALID_ID)
			return;

		const Node& grabbed{ GetGrabbedConst(nodes) };
		const RectFloat& rect{ grabbed.GetHeaderRect() };
		const Float2 mousePos{ camera.GetMouseWorldPos() };
		m_GrabOffset = mousePos - rect.GetLeftBot();
	}
}

const Node& NodeGrabbing::GetGrabbedConst(const InvalidateList<Node>& nodes) const
{
	return nodes.Get(m_Grabbed);
}

Node& NodeGrabbing::GetGrabbed(InvalidateList<Node>& nodes) const
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
