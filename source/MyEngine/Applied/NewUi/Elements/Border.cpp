#include "pch.h"
#include "Border.h"

#include "..\NewUiSystem.h"

using namespace NewUi;

Border::Border()
	: ParentElem<EmptyChildOptions>{}
{;
}

void Border::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	ResizePref childPref;
	childPref.horMode = pref.horMode;
	childPref.verMode = pref.verMode;
	childPref.minSize = {};
	childPref.maxSize = pref.maxSize - NewUiSystem::BORDER_THICKNESS * 4;

	Float2 childBounds{};
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		UpdateChildSize(i, childPref);
		SetChildPosition(i, Float2{ NewUiSystem::BORDER_THICKNESS * 2 });

		childBounds.x = Float::Max(childBounds.x, GetChild(i).GetWidth());
		childBounds.y = Float::Max(childBounds.y, GetChild(i).GetHeight());
	}

	SetSize(childBounds + NewUiSystem::BORDER_THICKNESS * 4);
}

void Border::Clear()
{
	m_BorderId.Clear();
	m_BackgroundId.Clear();
}

void Border::Create()
{
	m_BorderId = UI_RECT.Add({ GetPosition(), GetSize() }, NewUiSystem::COLOR_MEDIUM);
	m_BackgroundId = UI_RECT.Add({ GetPosition() + NewUiSystem::BORDER_THICKNESS, GetSize() - NewUiSystem::BORDER_THICKNESS * 2 }, NewUiSystem::COLOR_DARK);
}
