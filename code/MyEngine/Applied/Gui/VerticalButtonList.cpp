#include "pch.h"
#include "VerticalButtonList.h"

#include "Gui/FontRenderer.h"
#include "Gui/GuiRenderer.h"

using namespace Applied;

VerticalButtonList::VerticalButtonList(const Settings& settings)
	: m_Settings{ settings }
{
}

int VerticalButtonList::AddButton(const std::string& text)
{
	if (IsVisible())
	{
		Hide();
		const int buttonId{ m_Buttons.Add({ text }) };
		Show();
		return buttonId;
	}
	return m_Buttons.Add({ text });
}

void VerticalButtonList::RemoveButton(int buttonId)
{
	if (IsVisible())
	{
		Hide();
		m_Buttons.Remove(buttonId);
		Show();
	}
	else
		m_Buttons.Remove(buttonId);
}

void VerticalButtonList::Show()
{
	if (IsVisible()) return;

	//TEXT SIZES
	Float2 maxTextSize{ 40,20 };
	Array<Float2> textSizes{ m_Buttons.GetSize() };
	for (unsigned i = 0; i < m_Buttons.GetSize(); i++)
	{
		Button& button{ m_Buttons.GetSinceStart(i) };
		if (!button.IsValid())
			continue;

		const Float2 size{ FONT.GetSize(button.text, m_Settings.textHeight) };
		textSizes[i] = size;
		if (size.x > maxTextSize.x)
			maxTextSize.x = size.x;
		if (size.y > maxTextSize.y)
			maxTextSize.y = size.y;
	}

	//CALCULATE GUI VALUES
	const Float2 buttonInnerSize{ maxTextSize + m_Settings.buttonMargin * 2 };
	const Float2 buttonOuterSize{ buttonInnerSize + m_Settings.borderSize * 2 };
	const float interButtonMargin{ m_Settings.borderSize * 2 };
	const Float2 allButtonsSize{ buttonOuterSize.x, buttonOuterSize.y * m_Buttons.GetSize() + interButtonMargin * (m_Buttons.GetSize() - 1) };
	const Float2 innerSize{ allButtonsSize + interButtonMargin * 2 };
	const Float2 outerSize{ innerSize + m_Settings.borderSize * 2 };

	//MAKE MENU GUI
	m_BorderId = GUI.Add(m_Settings.pivot, {}, outerSize, m_Settings.borderColor);
	m_InnerId = GUI.Add(m_Settings.pivot, {}, innerSize, m_Settings.innerColor);

	//MAKE BUTTONS GUI
	Float2 buttonOffset{ 0, allButtonsSize.y / 2 - buttonOuterSize.y / 2 };
	for (unsigned i = 0; i < m_Buttons.GetSize(); i++)
	{
		Button& button{ m_Buttons.GetSinceStart(i) };
		button.borderId = GUI.Add(m_Settings.pivot, buttonOffset, buttonOuterSize, m_Settings.borderColor);
		button.innerId = GUI.Add(m_Settings.pivot, buttonOffset, buttonInnerSize, m_Settings.innerColor);

		//text
		const Float2& textSize{ textSizes[i] };
		button.textId = FONT.Add(button.text, m_Settings.pivot, buttonOffset, m_Settings.textHeight, m_Settings.borderColor);

		buttonOffset.y -= buttonOuterSize.y + interButtonMargin;
	}
}

void VerticalButtonList::Hide()
{
	//Menu
	GUI.Remove(m_BorderId);
	GUI.Remove(m_InnerId);
	m_BorderId = -1;

	//Buttons
	for (unsigned i = 0; i < m_Buttons.GetSize(); i++)
	{
		const Button& button{ m_Buttons.GetSinceStart(i) };
		GUI.Remove(button.borderId);
		GUI.Remove(button.innerId);
		FONT.Remove(button.textId);
	}
}

void VerticalButtonList::SetVisible(bool isVisible)
{
	if (isVisible)
		Show();
	else
		Hide();
}
