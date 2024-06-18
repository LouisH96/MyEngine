#include "pch.h"
#include "RendererD2D.h"

#include <Rendering/Dx/DxHelper.h>
#include "App/Win32/Window.h"

MyEngine::App::Wrappers::Dx::RendererD2D::RendererD2D(Win32::Window& window, D2D1::ColorF clearColor)
	: m_ClearColor(clearColor)
{
	//FACTORY
	D2D1_FACTORY_OPTIONS options{};
#if defined(DEBUG) || defined(_DEBUG)
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &m_pFactory);

	//RENDERTARGET
	const D2D1_SIZE_U size = D2D1::SizeU(window.GetClientWidth(), window.GetClientHeight());
	m_pFactory->CreateHwndRenderTarget({},
		D2D1::HwndRenderTargetProperties(window.GetWindowHandle(), size),
		&m_pRenderTarget);
}

MyEngine::App::Wrappers::Dx::RendererD2D::~RendererD2D()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pFactory);
}

ID2D1SolidColorBrush* MyEngine::App::Wrappers::Dx::RendererD2D::CreateBrush(D2D1::ColorF color) const
{
	ID2D1SolidColorBrush* pBrush;
	m_pRenderTarget->CreateSolidColorBrush(color, &pBrush);
	return pBrush;
}

void MyEngine::App::Wrappers::Dx::RendererD2D::Clear() const
{
	m_pRenderTarget->Clear(m_ClearColor);
}

void MyEngine::App::Wrappers::Dx::RendererD2D::Clear(D2D1::ColorF color) const
{
	m_pRenderTarget->Clear(color);
}

void MyEngine::App::Wrappers::Dx::RendererD2D::SetClearColor(const D2D1::ColorF& clearColor)
{
	m_ClearColor = clearColor;
}
