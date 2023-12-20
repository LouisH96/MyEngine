#include "pch.h"
#include "SimpleUiSystem.h"

#include "App/Win32/Window.h"

using namespace SimpleUi;

SimpleUiSystem::SimpleUiSystem()
	: m_Root{ RectFloat{{}, WINDOW.GetClientSize()} }
{
}
