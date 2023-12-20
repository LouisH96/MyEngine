#include "pch.h"
#include "Box.h"

#include "Gui/GuiRenderer.h"

using namespace SimpleUi;

Box::Box(const RectFloat& bounds, const Float3& color)
	: Elem{ bounds }
	, m_Color{ color }
{
}

void Box::Clear()
{
	GUI.Remove(m_BoxId);
}

void Box::Make()
{
	m_BoxId = GUI.Add({ -1,-1 }, m_Bounds.GetLeftBot(), m_Bounds.GetSize(), m_Color);
}

void Box::OnChildAdded(Elem& newChild)
{
	newChild.SetBounds({
		newChild.GetBounds().GetLeftBot() + m_Bounds.GetLeftBot(),
		newChild.GetBounds().GetSize() });
}

void Box::OnBoundsChanged(const RectFloat& oldBounds)
{
	const Float2 displacement{ GetLeftBot() - oldBounds.GetLeftBot() };

	Elem* pChild{ GetFirstChild() };
	while (pChild)
	{
		pChild->Move(displacement);
		pChild = pChild->GetNextSibling();
	}
}
