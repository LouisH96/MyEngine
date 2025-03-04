#include "pch.h"
#include "ToggleBtnBase.h"

ToggleBtnBase::ToggleBtnBase(bool isOn)
	: Elem{ true }
	, m_IsOn{ isOn }
{
}

void ToggleBtnBase::OnClick()
{
	m_HasChanged = true;
	m_IsOn = !m_IsOn;
}

bool ToggleBtnBase::HasChanged()
{
	const bool hasChanged{ m_HasChanged };
	m_HasChanged = false;
	return hasChanged;
}

void ToggleBtnBase::TreeUpdate(const ResizePref& pref)
{
	SetSize(pref);
}
