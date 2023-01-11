#include "pch.h"
#include "Screen.h"
#include "Window.h"

MyEngine::App::Win32::Screen MyEngine::App::Win32::Screen::m_Instance = {};

MyEngine::App::Win32::Screen::Screen()
{
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);

	m_Rect = Math::RectInt(0, 0, rect.right, rect.bottom);
}
