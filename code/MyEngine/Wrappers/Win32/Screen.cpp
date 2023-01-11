#include "pch.h"
#include "Screen.h"
#include "Window.h"

MyEngine::Wrappers::Win32::Screen MyEngine::Wrappers::Win32::Screen::m_Instance = {};

MyEngine::Wrappers::Win32::Screen::Screen()
{
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);

	m_Rect = Math::RectInt(0, 0, rect.right, rect.bottom);
}
