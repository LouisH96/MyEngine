#include "pch.h"
#include "Border.h"

#include "..\UiSystem.h"

using namespace Ui;

Border::Border()
	: ParentElem<EmptyChildOptions>{}
	, m_BackgroundColor{ UiSystem::COLOR_DARK }
	, m_BorderColor{ UiSystem::COLOR_MEDIUM }
{;
}

void Border::SetBorderColor(const Float3& borderColor)
{
	m_BorderColor = borderColor;
	m_BorderId.SetColor(borderColor);
}

void Border::SetBackgroundColor(const Float3& backgroundColor)
{
	m_BackgroundColor = backgroundColor;
	m_BackgroundId.SetColor(backgroundColor);
}

void Border::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	ResizePref childPref;
	childPref.horMode = pref.horMode;
	childPref.verMode = pref.verMode;
	childPref.minSize = {};
	childPref.maxSize = pref.maxSize - BORDER_THICKNESS * 4;

	Float2 childBounds{};
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		ChildUpdateSizeAndTreePositions(i, childPref);
		ChildSetPosition(i, Float2{ BORDER_THICKNESS * 2 });

		childBounds.x = Float::Max(childBounds.x, GetChild(i).GetWidth());
		childBounds.y = Float::Max(childBounds.y, GetChild(i).GetHeight());
	}

	SetSize(childBounds + BORDER_THICKNESS * 4);
}

void Border::Clear()
{
	m_BorderId.Clear();
	m_BackgroundId.Clear();
}

void Border::Create()
{
	m_BorderId = UI_RECT.Add({ GetPosition(), GetSize() }, m_BorderColor);
	m_BackgroundId = UI_RECT.Add({ GetPosition() + BORDER_THICKNESS, GetSize() - BORDER_THICKNESS * 2 }, m_BackgroundColor);
}
