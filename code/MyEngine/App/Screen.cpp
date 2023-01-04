#include "pch.h"
#include "Screen.h"

#include "Win32/Win32Window.h"

MyEngine::App::Screen MyEngine::App::Screen::m_Instance = {};

MyEngine::App::Screen::Screen()
{
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);

	m_Rect = Math::RectInt(0, 0, rect.right, rect.bottom);
}
