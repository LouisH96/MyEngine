#include "pch.h"
#include "Label.h"

#include "..\UiSystem.h"

//#define MY_DEBUG

using namespace Ui;

Label::Label(const std::string& text, float fontSize)
	: Label{ text, UiSystem::COLOR_MEDIUM, fontSize }
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

#ifdef MY_DEBUG
	AssertWithinMaxSize(textSize, pref);
#else
	UNREFERENCED_PARAMETER(pref);
#endif
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

#undef MY_DEBUG