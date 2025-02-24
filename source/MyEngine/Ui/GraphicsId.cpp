#include "pch.h"
#include "GraphicsId.h"

RectId::RectId(unsigned id)
	: GraphicsId{ id }
{
}

void RectId::SetColor(const Float3& newColor)
{
	GetRenderer().SetColor(m_Id, newColor);
}
