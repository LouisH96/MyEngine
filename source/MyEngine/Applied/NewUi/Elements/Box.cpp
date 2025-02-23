#include "pch.h"
#include "Box.h"

#include "..\NewUiSystem.h"

using namespace NewUi;

Box::Box(const Float3& color)
	: m_Color{ color }
{
}

void Box::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	SetSize(pref);
}

const std::string Box::GetTypeName() const
{
	return "Box";
}

void Box::Clear()
{
	m_BoxId.Clear();
}

void Box::Create()
{
	m_BoxId = UI_RECT.Add(
		{ GetBounds().GetLeftBot(), GetBounds().GetSize() }, m_Color);
}
