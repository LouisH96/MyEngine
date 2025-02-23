#include "pch.h"
#include "Label.h"

#include "Applied/NewUi/NewUiSystem.h"

using namespace NewUi;

Label::Label(const std::string& text, float fontSize)
	: Label{ text, NewUiSystem::COLOR_MEDIUM, fontSize }
{
}

Label::Label(const std::string& text, const Float3& color, float fontSize)
	: m_Text{ text }
	, m_Color{ color }
	, m_FontSize{ fontSize }
{
}

void Label::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	const Float2 textSize{ UI_FONT.GetTextSize_XCenter(m_Text, m_FontSize) };

	AssertWithinMaxSize(textSize, pref);
	SetSize(textSize);
}

void Label::Clear()
{
	m_TextId.Clear();
}

void Label::Create()
{
	m_TextId = UI_FONT.Add_XCenter({ m_Text, m_FontSize, m_Color }, GetPosition());
}
