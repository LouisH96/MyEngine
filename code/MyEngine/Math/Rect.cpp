#include "pch.h"
#include "Rect.h"

Math::Rect::Rect() = default;
Math::Rect::Rect(const Float2 & leftBot, const Float2 & size) : m_LeftBot{ leftBot }, m_Size{ size } {}
Math::Rect::Rect(const Float2 & leftBot, float width, float height) : m_LeftBot{ leftBot }, m_Size{ width, height } {}
Math::Rect::Rect(float left, float bot, const Float2 & size) : m_LeftBot{ left, bot }, m_Size{ size } {}

Float2 Rect::GetLeftTop() const
{
	return { m_LeftBot.x, m_LeftBot.y + m_Size.y };
}

Float2 Rect::GetRightBot() const
{
	return { m_LeftBot.x + m_Size.x, m_LeftBot.y };
}

Float2 Rect::GetRightTop() const
{
	return m_LeftBot + m_Size;
}
