#include "pch.h"
#include "Border.h"

#include "..\UiSystem.h"

using namespace Ui;

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
	childPref.maxSize = pref.maxSize - UiSystem::BORDER_THICKNESS * 4;

	Float2 childBounds{};
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		UpdateChildSize(i, childPref);
		SetChildPosition(i, Float2{ UiSystem::BORDER_THICKNESS * 2 });

		childBounds.x = Float::Max(childBounds.x, GetChild(i).GetWidth());
		childBounds.y = Float::Max(childBounds.y, GetChild(i).GetHeight());
	}

	SetSize(childBounds + UiSystem::BORDER_THICKNESS * 4);
}

void Border::Clear()
{
	m_BorderId.Clear();
	m_BackgroundId.Clear();
}

void Border::Create()
{
	m_BorderId = UI_RECT.Add({ GetPosition(), GetSize() }, UiSystem::COLOR_MEDIUM);
	m_BackgroundId = UI_RECT.Add({ GetPosition() + UiSystem::BORDER_THICKNESS, GetSize() - UiSystem::BORDER_THICKNESS * 2 }, UiSystem::COLOR_DARK);
}
