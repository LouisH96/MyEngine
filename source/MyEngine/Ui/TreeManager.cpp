#include "pch.h"
#include "TreeManager.h"

#include <Rendering\Canvas.h>
#include "Elem.h"

using namespace Ui;

TreeManager::TreeManager()
	: m_Root{}
{
}

void TreeManager::RecreateTree()
{
	m_NeedUpdate = false;

	ClearGraphics();

	Elem& root{ m_Root };
	root.UpdateSizeAndTreePositions(GetRootResizePref());
	root.UpdateTreePositions({ 0,0 });

	CreateGraphics();
}

Elem* TreeManager::GetElemUnderMouse()
{
	const Float2 mouse{
		static_cast<float>(MOUSE.GetPos().x),
		m_Root.GetHeight() - MOUSE.GetPos().y };

	return RootElem().GetElemAt(mouse);
}

void TreeManager::ClearGraphics()
{
	RootElem().ClearTree();
}

void TreeManager::CreateGraphics()
{
	RootElem().CreateTree();
}

Elem& TreeManager::RootElem()
{
	return static_cast<Elem&>(m_Root);
}

const Elem& TreeManager::RootElem() const
{
	return static_cast<const Elem&>(m_Root);
}

ResizePref TreeManager::GetRootResizePref() const
{
	ResizePref pref{};
	pref.minSize = { 0,0 };
	pref.maxSize = CANVAS.GetSize();
	pref.SetMax();
	return pref;
}
