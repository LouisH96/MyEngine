#include "pch.h"
#include "RectInt.h"

MyEngine::Math::RectInt::RectInt()
	: m_X{ 0 }, m_Y{ 0 }, m_Width{ 0 }, m_Height{ 0 }
{}

MyEngine::Math::RectInt::RectInt(int x, int y, int width, int height)
	: m_X{ x }
	, m_Y{ y }
	, m_Width{ width }
	, m_Height{ height }
{
}
