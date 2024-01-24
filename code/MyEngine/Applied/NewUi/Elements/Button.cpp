#include "pch.h"
#include "Button.h"

#include "Gui/FontRenderer.h"
#include "Gui/GuiRenderer.h"

using namespace NewUi;

const Float3 Button::COLOR_MAIN = Float3{ .1f };
const Float3 Button::COLOR_SECOND = Float3{ .4f };
const Float2 Button::MARGIN{ 10.f, 8.f };

Button::Button(const std::string& text, float fontSize)
	: m_Text{ std::move(text) }
	, m_FontSize{ fontSize }
{

}

void Button::UpdateSizeAndTreePositions(const ResizePref&)
{
	const Float2 textSize{ FONT.GetSize(m_Text, m_FontSize) };
	const Float2 insideSize{ textSize + MARGIN * 2 };
	const Float2 borderSize{ insideSize + Float2{BORDER_THICKNESS}*2 };

	SetSize(borderSize);
}

void Button::Clear()
{
	GUI.Remove(m_BorderId);
	GUI.Remove(m_BackgroundId);
	FONT.Remove(m_TextId);
}

void Button::Create()
{
	const Float2 bgSize{ GetSize() - Float2{ BORDER_THICKNESS }*2 };
	const Float2 bgPos{ GetPosition() + Float2{ BORDER_THICKNESS } };

	const Float2 textPos{ bgPos + MARGIN };

	m_BorderId = GUI.Add({ -1,-1 }, GetPosition(), GetSize(), COLOR_SECOND);
	m_BackgroundId = GUI.Add({ -1,-1 }, bgPos, bgSize, COLOR_MAIN);
	m_TextId = FONT.Add(m_Text, { -1,-1 }, {textPos}, m_FontSize, COLOR_SECOND);
}
