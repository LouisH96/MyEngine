#include "pch.h"
#include "Extender.h"

using namespace Ui;

Extender::Extender(const SizeDef& sizeDef)
	: m_SizeDef{ sizeDef }
{
}

void Extender::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	SetSize(m_SizeDef.GetSize(pref.maxSize));

	ResizePref childPref;
	childPref.SetMax();
	childPref.minSize = {};
	childPref.maxSize = GetSize();

	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		UpdateChildSize(i, childPref);
		SetChildPosition(i, {});
	}
}

void Extender::SetSizeDef(const Float2& value)
{
	m_SizeDef.Value = value;
	RequestUpdate();
}

void Extender::SetSizeDefX(float valueX)
{
	m_SizeDef.Value.x = valueX;
	RequestUpdate();
}

void Extender::SetSizeDefY(float valueY)
{
	m_SizeDef.Value.y = valueY;
	RequestUpdate();
}

void Extender::Clear()
{
}

void Extender::Create()
{
}
