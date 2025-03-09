#include "pch.h"
#include "DynamicLabel.h"

using namespace Ui;

//#define MY_DEBUG
DynamicLabel::DynamicLabel(const std::string& text, const Float2& size, const Float3& color, float fontSize)
	: m_Text{ text }
	, m_Color{ color }
	, m_FontSize{ fontSize }
{
	SetSize(size);
}

void DynamicLabel::UpdateText()
{
	m_TextId.SetText(m_Text);
}

void DynamicLabel::SetPivot(const Float2& pivot)
{
	m_Pivot = pivot;
}

void DynamicLabel::SetColor(const Float3& color)
{
	m_Color = color;
	m_TextId.SetColor(m_Color);
}

void DynamicLabel::TreeUpdate(const ResizePref&)
{
}

void DynamicLabel::Clear()
{
	m_TextId.Clear();
}

void DynamicLabel::Create()
{
	UiFontRenderer::TextInfo text;
	text.Text = m_Text;
	text.Scale = m_FontSize;
	text.Color = m_Color;
	text.Pivot = m_Pivot;
	text.Position = GetPosition() + GetSize() * m_Pivot;
	m_TextId = UI_FONT.Add(text);
}

#undef MY_DEBUG