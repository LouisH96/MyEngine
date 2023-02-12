#include "pch.h"
#include "Rect.h"

Math::Rect::Rect() = default;
Math::Rect::Rect(const Float2 & leftBot, const Float2 & size) : m_LeftBot{ leftBot }, m_Size{ size } {}
Math::Rect::Rect(const Float2 & leftBot, float width, float height) : m_LeftBot{ leftBot }, m_Size{ width, height } {}
Math::Rect::Rect(float left, float bot, const Float2 & size) : m_LeftBot{ left, bot }, m_Size{ size } {}
