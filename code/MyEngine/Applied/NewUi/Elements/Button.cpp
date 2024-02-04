#include "pch.h"
#include "Button.h"

#include "Applied/NewUi/NewUiFontRenderer.h"
#include "Applied/NewUi/NewUiSystem.h"
#include "Gui/GuiRenderer.h"

using namespace NewUi;

const Float2 Button::MARGIN{ 15.f, 11.f };

Button::Button(const std::string& text, Function function, FunctionArg functionArg, float fontSize)
	: m_Text{ std::move(text) }
	, m_FontSize{ fontSize }
	, m_Function{ function }
	, m_FunctionArg{ functionArg }
{

}

void Button::ToDefaultState()
{
	GUI.SetColor(m_BorderId, NewUiSystem::COLOR_MEDIUM);
	NEW_FONT.EditColor(m_TextId, NewUiSystem::COLOR_MEDIUM);
}

void Button::ToHoverState()
{
	GUI.SetColor(m_BorderId, NewUiSystem::COLOR_LIGHT);
	NEW_FONT.EditColor(m_TextId, NewUiSystem::COLOR_LIGHT);
}

void Button::ToPressedState()
{
	GUI.SetColor(m_BorderId, NewUiSystem::COLOR_MEDIUM);
	NEW_FONT.EditColor(m_TextId, NewUiSystem::COLOR_LIGHT);
}

void Button::OnClick()
{
	m_Function(m_FunctionArg);
}

void Button::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	const Float2 textSize{ NEW_FONT.GetTextSize_XCenter(m_Text, m_FontSize) };
	const Float2 insideSize{ textSize + MARGIN * 2 };
	Float2 borderSize{ insideSize + Float2{NewUiSystem::BORDER_THICKNESS}*2 };

	if (pref.horMode == Max)
		borderSize.x = pref.maxSize.x;

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
	const Float2 bgSize{ GetSize() - Float2{ NewUiSystem::BORDER_THICKNESS }*2 };
	const Float2 bgPos{ GetPosition() + Float2{ NewUiSystem::BORDER_THICKNESS } };

	const Float2 textSize{ NEW_FONT.GetTextSize_XCenter(m_Text, m_FontSize) };
	const Float2 textPos{ (GetSize() - textSize) / 2.f + GetPosition() };

	m_BorderId = GUI.Add({ -1,-1 }, GetPosition(), GetSize(), NewUiSystem::COLOR_MEDIUM);
	m_BackgroundId = GUI.Add({ -1,-1 }, bgPos, bgSize, NewUiSystem::COLOR_DARK);
	m_TextId = NEW_FONT.Add_XCenter({ m_Text, m_FontSize, NewUiSystem::COLOR_MEDIUM }, textPos);
}

const std::string Button::GetTypeName() const
{
	return "Button";
}
