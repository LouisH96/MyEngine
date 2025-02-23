#include "pch.h"
#include "Button.h"

#include "..\NewUiFontRenderer.h"
#include "..\NewUiSystem.h"

using namespace NewUi;

const Float2 Button::MARGIN{ 15.f, 11.f };

Button::Button(const std::string& text, Function function, FunctionArg functionArg, float fontSize)
	: m_Text{ text }
	, m_FontSize{ fontSize }
	, m_Function{ function }
	, m_FunctionArg{ functionArg }
{
}

void Button::ToDefaultState()
{
	UI_RECT.SetColor(m_BorderId.GetId(), NewUiSystem::COLOR_MEDIUM);
	UI_FONT.EditColor(m_TextId.GetId(), NewUiSystem::COLOR_MEDIUM);
}

void Button::ToHoverState()
{
	UI_RECT.SetColor(m_BorderId.GetId(), NewUiSystem::COLOR_LIGHT);
	UI_FONT.EditColor(m_TextId.GetId(), NewUiSystem::COLOR_LIGHT);
}

void Button::ToPressedState()
{
	UI_RECT.SetColor(m_BorderId.GetId(), NewUiSystem::COLOR_MEDIUM);
	UI_FONT.EditColor(m_TextId.GetId(), NewUiSystem::COLOR_LIGHT);
}

void Button::OnClick()
{
	m_Function(m_FunctionArg);
}

void Button::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	const Float2 textSize{ UI_FONT.GetTextSize_XCenter(m_Text, m_FontSize) };
	const Float2 insideSize{ textSize + MARGIN * 2 };
	Float2 borderSize{ insideSize + Float2{NewUiSystem::BORDER_THICKNESS}*2 };

	if (pref.horMode == FillMode::Max)
		borderSize.x = pref.maxSize.x;

	AssertWithinMaxSize(borderSize, pref);
	SetSize(borderSize);
}

void Button::Clear()
{
	m_BorderId.Clear();
	m_BackgroundId.Clear();
	m_TextId.Clear();
}

void Button::Create()
{
	const Float2 bgSize{ GetSize() - Float2{ NewUiSystem::BORDER_THICKNESS }*2 };
	const Float2 bgPos{ GetPosition() + Float2{ NewUiSystem::BORDER_THICKNESS } };

	const Float2 textSize{ UI_FONT.GetTextSize_XCenter(m_Text, m_FontSize) };
	const Float2 textPos{ (GetSize() - textSize) / 2.f + GetPosition() };

	m_BorderId = UI_RECT.Add({ GetPosition(), GetSize() }, NewUiSystem::COLOR_MEDIUM);
	m_BackgroundId = UI_RECT.Add({bgPos, bgSize}, NewUiSystem::COLOR_DARK);
	m_TextId = UI_FONT.Add_XCenter({ m_Text, m_FontSize, NewUiSystem::COLOR_MEDIUM }, textPos);
}

const std::string Button::GetTypeName() const
{
	return "Button";
}
