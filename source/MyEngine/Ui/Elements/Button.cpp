#include "pch.h"
#include "Button.h"

#include "..\UiSystem.h"
#include "..\Renderers\UiFontRenderer.h"

//#define MY_DEBUG

using namespace Ui;

const Float2 Button::MARGIN{ 15.f, 11.f };

Button::Button(const std::string& text, Function function, FunctionArg functionArg, float fontSize)
	: Elem{ true }
	, m_Text{ text }
	, m_FontSize{ fontSize }
	, m_Function{ function }
	, m_FunctionArg{ functionArg }
{
}

void Button::ToDefaultState()
{
	UI_RECT.SetColor(m_BorderId.GetId(), UiSystem::COLOR_MEDIUM);
	UI_FONT.EditColor(m_TextId.GetId(), UiSystem::COLOR_MEDIUM);
}

void Button::ToHoverState()
{
	UI_RECT.SetColor(m_BorderId.GetId(), UiSystem::COLOR_LIGHT);
	UI_FONT.EditColor(m_TextId.GetId(), UiSystem::COLOR_LIGHT);
}

void Button::ToPressedState()
{
	UI_RECT.SetColor(m_BorderId.GetId(), UiSystem::COLOR_MEDIUM);
	UI_FONT.EditColor(m_TextId.GetId(), UiSystem::COLOR_LIGHT);
}

void Button::OnClick()
{
	m_Function(m_FunctionArg);
}

void Button::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	const Float2 textSize{ UI_FONT.GetTextSize_XCenter(m_Text, m_FontSize) };
	const Float2 insideSize{ textSize + MARGIN * 2 };
	Float2 borderSize{ insideSize + Float2{UiSystem::BORDER_THICKNESS}*2 };

	if (pref.horMode == FillMode::Max)
		borderSize.x = pref.maxSize.x;

#ifdef MY_DEBUG
	AssertWithinMaxSize(borderSize, pref);
#endif
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
	const Float2 bgSize{ GetSize() - Float2{ UiSystem::BORDER_THICKNESS }*2 };
	const Float2 bgPos{ GetPosition() + Float2{ UiSystem::BORDER_THICKNESS } };

	const Float2 textSize{ UI_FONT.GetTextSize_XCenter(m_Text, m_FontSize) };
	const Float2 textPos{ (GetSize() - textSize) / 2.f + GetPosition() };

	m_BorderId = UI_RECT.Add({ GetPosition(), GetSize() }, UiSystem::COLOR_MEDIUM);
	m_BackgroundId = UI_RECT.Add({ bgPos, bgSize }, UiSystem::COLOR_DARK);
	m_TextId = UI_FONT.Add_XCenter({ m_Text, m_FontSize, UiSystem::COLOR_MEDIUM }, textPos);
}

const std::string Button::GetTypeName() const
{
	return "Button";
}

#undef MY_DEBUG
