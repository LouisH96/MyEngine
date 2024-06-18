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
	const Float2 textSize{ NEW_FONT.GetTextSize_XCenter(m_Text, m_FontSize) };

	AssertWithinMaxSize(textSize, pref);
	SetSize(textSize);
}

void Label::Clear()
{
	NEW_FONT.Remove(m_TextId);
}

void Label::Create()
{
	m_TextId = NEW_FONT.Add_XCenter({ m_Text, m_FontSize, m_Color }, GetPosition());
}
