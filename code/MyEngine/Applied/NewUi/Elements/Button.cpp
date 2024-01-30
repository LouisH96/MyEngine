#include "pch.h"
#include "Button.h"

#include "Applied/NewUi/NewUiFontRenderer.h"
#include "Applied/NewUi/NewUiSystem.h"
#include "Gui/GuiRenderer.h"

using namespace NewUi;

const Float3 Button::COLOR_DARK = Float3{ .1f };
const Float3 Button::COLOR_MEDIUM = Float3{ .4f };
const Float3 Button::COLOR_LIGHT = Float3{ .6f };
const Float2 Button::MARGIN{ 15.f, 11.f };

Button::Button(const std::string& text, float fontSize)
	: m_Text{ std::move(text) }
	, m_FontSize{ fontSize }
{

}

void Button::ToDefaultState()
{
	GUI.SetColor(m_BorderId, COLOR_MEDIUM);
	NEW_FONT.EditColor(m_TextId, COLOR_MEDIUM);
}

void Button::ToHoverState()
{
	GUI.SetColor(m_BorderId, COLOR_LIGHT);
	NEW_FONT.EditColor(m_TextId, COLOR_LIGHT);
}

void Button::ToPressedState()
{
	GUI.SetColor(m_BorderId, COLOR_MEDIUM);
	NEW_FONT.EditColor(m_TextId, COLOR_LIGHT);
}

void Button::OnClick()
{
	std::cout << "Button clicked\n";
}

void Button::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	const Float2 textSize{ NEW_FONT.GetTextSize_XCenter(m_Text, m_FontSize) };
	const Float2 insideSize{ textSize + MARGIN * 2 };
	const Float2 borderSize{ insideSize + Float2{BORDER_THICKNESS}*2 };

	AssertWithinMaxSize(borderSize, pref);
	SetSize(borderSize);
}

void Button::Clear()
{
	GUI.Remove(m_BorderId);
	GUI.Remove(m_BackgroundId);
	NEW_FONT.Remove(m_TextId);
}

void Button::Create()
{
	const Float2 bgSize{ GetSize() - Float2{ BORDER_THICKNESS }*2 };
	const Float2 bgPos{ GetPosition() + Float2{ BORDER_THICKNESS } };

	const Float2 textPos{ bgPos + MARGIN };

	m_BorderId = GUI.Add({ -1,-1 }, GetPosition(), GetSize(), COLOR_MEDIUM);
	m_BackgroundId = GUI.Add({ -1,-1 }, bgPos, bgSize, COLOR_DARK);
	m_TextId = NEW_FONT.Add_XCenter({ m_Text, m_FontSize, COLOR_MEDIUM }, textPos);
}

const std::string Button::GetTypeName() const
{
	return "Button";
}
