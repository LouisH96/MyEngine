#include "pch.h"
#include "MouseDownElem.h"

using namespace Ui;

MouseDownElem::MouseDownElem()
	: Elem{ true }
{
}

void MouseDownElem::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	SetSize(pref);
}

void MouseDownElem::OnMouseDown(Float2 relativePos)
{
	m_IsMouseDown = true;
	m_RelPos = relativePos;
}

bool MouseDownElem::IsMouseDown()
{
	const bool down{ m_IsMouseDown};
	m_IsMouseDown = false;
	return down;
}
