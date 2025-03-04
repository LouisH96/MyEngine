#include "pch.h"
#include "GraphicsId.h"

RectId::RectId(unsigned id)
	: GraphicsId{ id }
{
}

void RectId::SetColor(const Float3& newColor)
{
	if (IsEmpty())
		return;
	GetRenderer().SetColor(m_Id, newColor);
}

void RectId::MoveTo(const Float2& leftBot)
{
	if (IsEmpty())
		return;
	GetRenderer().MoveTo(m_Id, leftBot);
}

Float3 RectId::GetColor() const
{
	if (IsEmpty())
		return {};
	return GetRenderer().GetColor(m_Id);
}

FontId::FontId(unsigned id)
	: GraphicsId{ id }
{
}

void FontId::SetColor(const Float3& newColor)
{
	if (IsEmpty())
		return;
	GetRenderer().EditColor(m_Id, newColor);
}

Float3 FontId::GetColor() const
{
	if (IsEmpty())
		return {};

	return GetRenderer().GetColor(m_Id);
}
