#include "pch.h"
#include "Renderer2D.h"

#include <App/Wrappers/Win32/Window.h>

MyEngine::App::Wrappers::Win32::Renderer2D::Renderer2D(MyEngine::App::Wrappers::Win32::Window& window)
	: m_Window(window)
	, m_WindowContext(GetDC(m_Window.GetWindowHandle()))
{
	// Initialize GDI+.
	const Gdiplus::GdiplusStartupInput gdiStartup{};
	GdiplusStartup(&m_GdiToken, &gdiStartup, nullptr);
	
	// Create BackBuffer
	m_BackBufferContext = CreateCompatibleDC(m_WindowContext);
	m_BackBufferBitmap = CreateCompatibleBitmap(m_WindowContext, m_Window.GetClientSize().x, m_Window.GetClientSize().y);
	m_Handle = SelectObject(m_BackBufferContext, m_BackBufferBitmap);
	m_pGraphics = new Gdiplus::Graphics(m_BackBufferContext);
}

MyEngine::App::Wrappers::Win32::Renderer2D::~Renderer2D()
{
	SelectObject(m_BackBufferContext, m_Handle);
	DeleteObject(m_BackBufferBitmap);
	DeleteDC(m_BackBufferContext);

	Gdiplus::GdiplusShutdown(m_GdiToken);
}

void MyEngine::App::Wrappers::Win32::Renderer2D::Present() const
{
	BitBlt(m_WindowContext, 0, 0, m_Window.GetClientSize().x, m_Window.GetClientSize().y, m_BackBufferContext, 0, 0, SRCCOPY);
}
