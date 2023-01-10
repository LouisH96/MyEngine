#include "pch.h"
#include "Screen.h"

#include "Window.h"

MyEngine::App::Screen MyEngine::App::Screen::m_Instance = {};

MyEngine::App::Screen::Screen()
{
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);

	m_Rect = Math::RectInt(0, 0, rect.right, rect.bottom);
}
