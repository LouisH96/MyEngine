#include "pch.h"
#include "Renderer2D.h"

#include <App/Win32/Window.h>

MyEngine::App::Win32::Renderer2D::Renderer2D(MyEngine::App::Win32::Window& window)
	: m_Window(window)
	, m_WindowContext(GetDC(m_Window.GetWindowHandle()))
{
	// Create BackBuffer
	m_BackBufferContext = CreateCompatibleDC(m_WindowContext);
	m_BackBufferBitmap = CreateCompatibleBitmap(m_WindowContext, m_Window.GetClientSize().x, m_Window.GetClientSize().y);
	m_Handle = SelectObject(m_BackBufferContext, m_BackBufferBitmap);
}

MyEngine::App::Win32::Renderer2D::~Renderer2D()
{
	SelectObject(m_BackBufferContext, m_Handle);
	DeleteObject(m_BackBufferBitmap);
	DeleteDC(m_BackBufferContext);
}

void MyEngine::App::Win32::Renderer2D::Present() const
{
	BitBlt(m_WindowContext, 0, 0, m_Window.GetClientSize().x, m_Window.GetClientSize().y, m_BackBufferContext, 0, 0, SRCCOPY);
}
