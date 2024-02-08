#include "pch.h"
#include "Extender.h"

using namespace NewUi;

Extender::Extender(const SizeDef& sizeDef)
	: m_SizeDef{ sizeDef }
{
}

void Extender::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	SetSize(m_SizeDef.GetSize(pref.maxSize));

	ResizePref childPref;
	childPref.horMode = Max;
	childPref.verMode = Max;
	childPref.minSize = {};
	childPref.maxSize = GetSize();

	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		UpdateChildSize(i, childPref);
		SetChildPosition(i, {});
	}
}

void Extender::Clear()
{
}

void Extender::Create()
{
}
