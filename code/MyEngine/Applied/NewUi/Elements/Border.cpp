#include "pch.h"
#include "Border.h"

#include "Applied/NewUi/NewUiSystem.h"
#include "Gui/GuiRenderer.h"

using namespace NewUi;

Border::Border(bool shrink)
	: m_Shrink{ shrink }
{
}

void Border::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	ResizePref childPref;
	childPref.horMode = m_Shrink ? Min : Max;
	childPref.verMode = m_Shrink ? Min : Max;
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

	if (m_Shrink)
		SetSize(childBounds + NewUiSystem::BORDER_THICKNESS * 4);
	else
		SetSize(pref.maxSize);
}

void Border::Clear()
{
	GUI.Remove(m_BorderId);
	GUI.Remove(m_BackgroundId);
}

void Border::Create()
{
	m_BorderId = GUI.Add({ -1,-1 }, GetPosition(), GetSize(), NewUiSystem::COLOR_MEDIUM);
	m_BackgroundId = GUI.Add({ -1,-1 }, GetPosition() + NewUiSystem::BORDER_THICKNESS, GetSize() - NewUiSystem::BORDER_THICKNESS * 2, NewUiSystem::COLOR_DARK);

}
