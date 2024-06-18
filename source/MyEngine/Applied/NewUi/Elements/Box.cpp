#include "pch.h"
#include "Box.h"

#include "Gui/GuiRenderer.h"

using namespace NewUi;

Box::Box(const Float3& color)
	: m_Color{ color }
{

}

void Box::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	SetSize(pref);

	//temp
	if (GetSize().x == 0)
		SetSize(Float2{ 100,100 } *Float2{ m_Color.x, m_Color.y });
}

const std::string Box::GetTypeName() const
{
	return "Box";
}

void Box::Clear()
{
	GUI.Remove(m_BoxId);
}

void Box::Create()
{
	m_BoxId = GUI.Add({ -1,-1 }, GetBounds().GetLeftBot(), GetBounds().GetSize(), m_Color);
}
