#include "pch.h"
#include "Canvas.h"

#include "Dx/DxHelper.h"
#include "Gpu.h"
#include "RenderOptions.h"
#include <App/Win32/Window.h>

#include <dxgi1_2.h>

using namespace Rendering;
using namespace Dx;

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
	Globals::pGpu->GetContext().OMSetRenderTargets(1, &m_pRenderTargetView, m_DepthStencilBuffer.GetView());
	m_Viewport.Activate();
}

Canvas::~Canvas()
{
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pRenderTargetTexture);
	SAFE_RELEASE(m_pSwapChainBuffer);
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
		m_pRenderTargetView, &m_Color.x);
	ClearDepthBuffer();
}

void Canvas::Present() const
{
	Globals::pGpu->GetContext().ResolveSubresource(
		m_pSwapChainBuffer,
		0,
		m_pRenderTargetTexture,
		0,
		SWAPCHAIN_FORMAT);

	DXGI_PRESENT_PARAMETERS param{ 0,nullptr,0,nullptr };

	const HRESULT result{ m_pSwapChain->Present1(1, 0, &param) };
	DxHelper::OnFail("[Canvas::Present]", result);
}

App::ResizedEvent Canvas::OnWindowResized(Int2 newSize)
{
	//Calculate
	const App::ResizedEvent event
	{
		m_Size,
		newSize,
	};
	m_Size = newSize;
	m_InvSize = { 1.f / newSize.x, 1.f / newSize.y };
	//Resize
	ResizeSwapChain();
	ResizeRenderTarget();
	//Notify
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

	HRESULT result{ pFactory->CreateSwapChainForHwnd(&Globals::pGpu->GetDevice(), window.GetWindowHandle(), &desc, nullptr, nullptr, &m_pSwapChain) };
	DxHelper::OnFail("[Canvas::InitSwapChain::CreateSwapChain]", result);
	result = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_pSwapChainBuffer));
	DxHelper::OnFail("[Canvas::InitSwapChain::GetBuffer]", result);

	pFactory->Release();
	pAdapter->Release();
	pDevice2->Release();
}

void Canvas::InitRenderTarget()
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = m_Size.x;
	desc.Height = m_Size.y;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = RenderOptions::Samples;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET;
	HRESULT result{ Globals::pGpu->GetDevice().CreateTexture2D(&desc, nullptr, &m_pRenderTargetTexture) };

	DxHelper::OnFail("Canvas::InitRenderTarget::CreateTexture", result);

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc{};
	viewDesc.Format = desc.Format;
	viewDesc.ViewDimension = RenderOptions::UsingMultiSampling() ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

	if (m_pRenderTargetTexture)
	{
		result =
			Globals::pGpu->GetDevice().CreateRenderTargetView(m_pRenderTargetTexture, &viewDesc, &m_pRenderTargetView);
		DxHelper::OnFail("Canvas::InitRenderTarget", result);
	}
}

void Canvas::ResizeSwapChain()
{
	m_pSwapChainBuffer->Release();
	HRESULT result;
	result = m_pSwapChain->ResizeBuffers(0, m_Size.x, m_Size.y, DXGI_FORMAT_UNKNOWN, 0);
	DxHelper::OnFail("[Canvas::OnWindowResized] failed resizing buffer", result);
	result = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_pSwapChainBuffer));
	DxHelper::OnFail("[Canvas::InitSwapChain::GetBuffer]", result);
}

void Canvas::ResizeRenderTarget()
{
	m_pRenderTargetTexture->Release();
	m_pRenderTargetView->Release();
	InitRenderTarget();
}

void Canvas::GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter,
	IDXGIFactory2*& pFactory) const
{
	HRESULT hr = Globals::pGpu->GetDevice().QueryInterface(__uuidof(IDXGIDevice2), reinterpret_cast<void**>(&pDevice2));
	DxHelper::OnFail("Canvas::GetFactory2::Device2", hr);

	hr = pDevice2->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pAdapter));
	DxHelper::OnFail("Canvas::GetFactory2::Adapter", hr);

	hr = pAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pFactory));
	DxHelper::OnFail("Canvas::GetFactory2::Factory", hr);
}
