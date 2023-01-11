#include "pch.h"
#include "DxCanvas.h"

#include "DxHelper.h"
#include "../../Wrappers/Win32/Window.h"

MyEngine::Gpu::Dx::DxCanvas::DxCanvas(DxGpu& gpu, Wrappers::Win32::Window& window)
	: m_Gpu{ gpu }
{
	InitSwapChain(window);
	InitRenderTarget();
	SetViewPort(window.GetSize_WinApi());
}

void MyEngine::Gpu::Dx::DxCanvas::Activate() const
{
	m_Gpu.GetContext().OMSetRenderTargets(1, &m_pMainRenderTargetView, nullptr);
}

MyEngine::Gpu::Dx::DxCanvas::~DxCanvas()
{
	SAFE_RELEASE(m_pMainRenderTargetView)
		SAFE_RELEASE(m_pSwapChain)
}

void MyEngine::Gpu::Dx::DxCanvas::Clear() const
{
	/* clear the back buffer to cornflower blue for the new frame */
	constexpr float background_colour[4] = {
	  0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
	m_Gpu.GetContext().ClearRenderTargetView(
		m_pMainRenderTargetView, background_colour);
}

void MyEngine::Gpu::Dx::DxCanvas::Present() const
{
	DXGI_PRESENT_PARAMETERS param{ 0,nullptr,0,nullptr };
	m_pSwapChain->Present1(0, DXGI_PRESENT_DO_NOT_WAIT, &param);
}

void MyEngine::Gpu::Dx::DxCanvas::OnWindowResized(DirectX::XMINT2 newSize)
{
	SAFE_RELEASE(m_pMainRenderTargetView);
	m_pSwapChain->ResizeBuffers(0, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0);
	InitRenderTarget();
	SetViewPort(newSize);
}

void MyEngine::Gpu::Dx::DxCanvas::InitSwapChain(const Wrappers::Win32::Window& window)
{
	const DirectX::XMINT2 windowSize = window.GetSize_WinApi();
	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.BufferCount = 2;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Scaling = DXGI_SCALING_NONE;
	desc.Width = windowSize.x;
	desc.Height = windowSize.y;

	IDXGIDevice2* pDevice2{};
	IDXGIAdapter* pAdapter{};
	IDXGIFactory2* pFactory{};
	GetFactory2(pDevice2, pAdapter, pFactory);

	if (pFactory->CreateSwapChainForHwnd(&m_Gpu.GetDevice(), window.GetWindowHandle(), &desc, nullptr, nullptr, &m_pSwapChain) != S_OK)
		throw std::exception("DxCanvas::InitSwapChain");

	pFactory->Release();
	pAdapter->Release();
	pDevice2->Release();
}

void MyEngine::Gpu::Dx::DxCanvas::InitRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
		m_Gpu.GetDevice().CreateRenderTargetView(pBackBuffer, nullptr, &m_pMainRenderTargetView);
	pBackBuffer->Release();
}

void MyEngine::Gpu::Dx::DxCanvas::SetViewPort(DirectX::XMINT2 windowSize)
{
	m_ViewPort = {
	  0.0f, 0.0f,
	 static_cast<float>(windowSize.x),
	 static_cast<float>(windowSize.y),
	  0.0f, 1.0f };
	m_Gpu.GetContext().RSSetViewports(1, &m_ViewPort);
}

void MyEngine::Gpu::Dx::DxCanvas::GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter,
	IDXGIFactory2*& pFactory) const
{
	HRESULT hr = m_Gpu.GetDevice().QueryInterface(__uuidof(IDXGIDevice2), reinterpret_cast<void**>(&pDevice2));
	if (FAILED(hr))
		throw std::exception("DxCanvas::GetFactory2::Device2");

	hr = pDevice2->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pAdapter));
	if (FAILED(hr))
		throw std::exception("DxCanvas::GetFactory2::Adapter");

	hr = pAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pFactory));
	if (FAILED(hr))
		throw std::exception("DxCanvas::GetFactory2::Factory");
}
