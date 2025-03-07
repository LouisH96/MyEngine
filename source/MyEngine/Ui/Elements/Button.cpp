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
	, m_BgColor{ UiSystem::COLOR_DARK }
	, m_BorderColor{ UiSystem::COLOR_MEDIUM }
	, m_InteractColor{ UiSystem::COLOR_LIGHT }
{
}

void Button::ToDefaultState()
{
	UI_RECT.SetColor(m_BorderId.GetId(), m_BorderColor);
	UI_FONT.EditColor(m_TextId.GetId(), m_BorderColor);
}

void Button::ToHoverState()
{
	UI_RECT.SetColor(m_BorderId.GetId(), m_InteractColor);
	UI_FONT.EditColor(m_TextId.GetId(), m_InteractColor);
}

void Button::ToPressedState()
{
	UI_RECT.SetColor(m_BorderId.GetId(), m_BorderColor);
	UI_FONT.EditColor(m_TextId.GetId(), m_InteractColor);
}

void Button::OnClick()
{
	m_Function(m_FunctionArg);
}

void Button::TreeUpdate(const ResizePref& pref)
{
	const Float2 textSize{ UI_FONT.GetTextSize(m_Text, m_FontSize) };
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

	const Float2 textSize{ UI_FONT.GetTextSize(m_Text, m_FontSize) };
	const Float2 textPos{ (GetSize() - textSize) / 2.f + GetPosition() };

	UiFontRenderer::TextInfo text{};
	text.Text = m_Text;
	text.Scale = m_FontSize;
	text.Color = m_BorderColor;
	text.Position = textPos;

	m_BorderId = UI_RECT.Add({ GetPosition(), GetSize() }, m_BorderColor);
	m_BackgroundId = UI_RECT.Add({ bgPos, bgSize }, m_BgColor);
	m_TextId = UI_FONT.Add(text);
}

const std::string Button::GetTypeName() const
{
	return "Button";
}

void Button::SetBgColor(const Float3& color)
{
	m_BgColor = color;
	m_BackgroundId.SetColor(color);
}

void Button::SetBorderColor(const Float3& color)
{
	if (m_BorderId.IsActive() &&
		m_BorderId.GetColor() == m_BorderColor)
	{
		m_BorderId.SetColor(color);
	}
	if (m_TextId.IsActive() &&
		m_TextId.GetColor() == m_BorderColor)
	{
		m_TextId.SetColor(color);
	}
	m_BorderColor = color;
}

void Button::SetInteractColor(const Float3& color)
{
	if (m_BorderId.IsActive() &&
		m_BorderId.GetColor() == m_InteractColor)
	{
		m_BorderId.SetColor(color);
	}
	if (m_TextId.IsActive() &&
		m_TextId.GetColor() == m_InteractColor)
	{
		m_TextId.SetColor(color);
	}
	m_InteractColor = color;
}

#undef MY_DEBUG
