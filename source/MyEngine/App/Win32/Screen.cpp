#include "pch.h"
#include "Screen.h"
#include "Window.h"

App::Win32::Screen App::Win32::Screen::m_Instance = {};

App::Win32::Screen::Screen()
{
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);

	m_Rect = RectInt{{}, {rect.right, rect.bottom} };
}
