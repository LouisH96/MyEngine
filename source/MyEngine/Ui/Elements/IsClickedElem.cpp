#include "pch.h"
#include "IsClickedElem.h"

using namespace Ui;

IsClickedElem::IsClickedElem()
	: Elem{ true }
{
}

bool IsClickedElem::IsClicked()
{
	const bool isClicked{ m_IsClicked };
	m_IsClicked = false;
	return isClicked;
}

void IsClickedElem::OnClick()
{
	m_IsClicked = true;
}

void IsClickedElem::TreeUpdate(const ResizePref& pref)
{
	SetSize(pref);
}