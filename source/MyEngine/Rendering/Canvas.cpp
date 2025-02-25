#include "pch.h"
#include "Canvas.h"

#include "Dx/DxHelper.h"
#include "Gpu.h"
#include <App/Win32/Window.h>

#include <dxgi1_2.h>

using namespace Rendering;

Canvas::Canvas(App::Win32::Window& window, const Float3& color)
	: m_Size{ window.GetClientSize() }
	, m_InvSize{ 1.f / window.GetClientWidthF(), 1.f / window.GetClientHeightF() }
	, m_Color{ color.x, color.y, color.z, 1 }
{
	InitSwapChain(window);
	InitRenderTarget();
	m_DepthStencilBuffer.Init(m_Size);
	m_Viewport = { m_Size };

	if (Globals::pCanvas) Logger::Error("Global canvas already set");
	Globals::pCanvas = this;
}

void Canvas::Activate()
{
	//Rendertarget
	Globals::pGpu->GetContext().OMSetRenderTargets(1, &m_pMainRenderTargetView, m_DepthStencilBuffer.GetView());
	m_Viewport.Activate();
}

Canvas::~Canvas()
{
	SAFE_RELEASE(m_pMainRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
}

void Canvas::ClearDepthBuffer()
{
	m_DepthStencilBuffer.Clear();
}

void Canvas::BeginPaint()
{
	Clear();
	Activate();
}

void Canvas::Clear()
{
	/* clear the back buffer to cornflower blue for the new frame */
	Globals::pGpu->GetContext().ClearRenderTargetView(
		m_pMainRenderTargetView, &m_Color.x);
	ClearDepthBuffer();
}

void Canvas::Present() const
{
	DXGI_PRESENT_PARAMETERS param{ 0,nullptr,0,nullptr };

	const HRESULT result{ m_pSwapChain->Present1(1, 0, &param) };
	if (FAILED(result))
		Logger::Error("SwapChain", result);
}

App::ResizedEvent Canvas::OnWindowResized(Int2 newSize)
{
	const App::ResizedEvent event
	{
		m_Size,
		newSize,
	};
	SAFE_RELEASE(m_pMainRenderTargetView);
	m_Size = newSize;
	m_InvSize = { 1.f / newSize.x, 1.f / newSize.y };
	const HRESULT result{ m_pSwapChain->ResizeBuffers(0, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0) };
	if (FAILED(result)) Logger::Error("[Canvas::OnWindowResized] failed resizing buffer");
	InitRenderTarget();
	m_DepthStencilBuffer.Update(m_Size);
	m_Viewport = { m_Size };
	return event;
}

void Canvas::InitSwapChain(const App::Win32::Window& window)
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.BufferCount = 2;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Scaling = DXGI_SCALING_NONE;
	desc.Width = m_Size.x;
	desc.Height = m_Size.y;

	IDXGIDevice2* pDevice2{};
	IDXGIAdapter* pAdapter{};
	IDXGIFactory2* pFactory{};
	GetFactory2(pDevice2, pAdapter, pFactory);

	if (pFactory->CreateSwapChainForHwnd(&Globals::pGpu->GetDevice(), window.GetWindowHandle(), &desc, nullptr, nullptr, &m_pSwapChain) != S_OK)
		Logger::Error("Canvas::InitSwapChain");

	pFactory->Release();
	pAdapter->Release();
	pDevice2->Release();
}

void Canvas::InitRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
		Globals::pGpu->GetDevice().CreateRenderTargetView(pBackBuffer, nullptr, &m_pMainRenderTargetView);
	pBackBuffer->Release();
}

void Canvas::GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter,
	IDXGIFactory2*& pFactory) const
{
	HRESULT hr = Globals::pGpu->GetDevice().QueryInterface(__uuidof(IDXGIDevice2), reinterpret_cast<void**>(&pDevice2));
	if (FAILED(hr))
		Logger::Error("Canvas::GetFactory2::Device2");

	hr = pDevice2->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pAdapter));
	if (FAILED(hr))
		Logger::Error("Canvas::GetFactory2::Adapter");

	hr = pAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pFactory));
	if (FAILED(hr))
		Logger::Error("Canvas::GetFactory2::Factory");
}
