#include "pch.h"
#include "Margin.h"

using namespace Ui;

Margin::Margin(float margin)
	: m_Margin{ margin }
{
}

void Margin::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	ResizePref childPref;
	childPref.horMode = pref.horMode;
	childPref.verMode = pref.verMode;
	childPref.minSize = {};
	childPref.maxSize = pref.maxSize - m_Margin * 2;

	Float2 childBounds{};
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		UpdateChildSize(i, childPref);
		SetChildPosition(i, Float2{ m_Margin });

		childBounds.x = Float::Max(childBounds.x, GetChild(i).GetWidth());
		childBounds.y = Float::Max(childBounds.y, GetChild(i).GetHeight());
	}

	SetSize(childBounds + m_Margin * 2);
}

void Margin::Clear()
{
}

void Margin::Create()
{
}
