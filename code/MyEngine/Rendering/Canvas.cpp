#include "pch.h"
#include "Canvas.h"

#include <dxgi1_2.h>

#include "Dx/DxHelper.h"
#include "App/Win32/Window.h"
#include "Gpu.h"

Rendering::Canvas::Canvas(App::Win32::Window& window)
	:m_Size{ window.GetClientSize() }
{
	InitSwapChain(window);
	InitRenderTarget();
	InitDepthStencil();
	SetViewPort();

	if (Globals::pCanvas) Logger::PrintError("Global canvas already set");
	Globals::pCanvas = this;
}

void Rendering::Canvas::Activate() const
{
	//Rendertarget
	Globals::pGpu->GetContext().OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pDepthStencilView);
}

Rendering::Canvas::~Canvas()
{
	SAFE_RELEASE(m_pMainRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDepthStencilView);
}

void Rendering::Canvas::BeginPaint() const
{
	Clear();
	Activate();
}

void Rendering::Canvas::Clear() const
{
	/* clear the back buffer to cornflower blue for the new frame */
	constexpr float background_colour[4] = {
	  0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
	Globals::pGpu->GetContext().ClearRenderTargetView(
		m_pMainRenderTargetView, background_colour);
	Globals::pGpu->GetContext().ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Rendering::Canvas::Present() const
{
	DXGI_PRESENT_PARAMETERS param{ 0,nullptr,0,nullptr };
	m_pSwapChain->Present1(0, DXGI_PRESENT_DO_NOT_WAIT, &param);
}

void Rendering::Canvas::OnWindowResized(Int2 newSize)
{
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pMainRenderTargetView);
	m_Size = newSize;
	m_pSwapChain->ResizeBuffers(0, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0);
	InitRenderTarget();
	InitDepthStencil();
	SetViewPort();
}

void Rendering::Canvas::InitSwapChain(const App::Win32::Window& window)
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
		Logger::PrintError("Canvas::InitSwapChain");

	pFactory->Release();
	pAdapter->Release();
	pDevice2->Release();
}

void Rendering::Canvas::InitRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
		Globals::pGpu->GetDevice().CreateRenderTargetView(pBackBuffer, nullptr, &m_pMainRenderTargetView);
	pBackBuffer->Release();
}

void Rendering::Canvas::InitDepthStencil()
{
	//TEXTURE
	ID3D11Texture2D* pTempTexture{};
	D3D11_TEXTURE2D_DESC descDepth{};
	descDepth.Width = m_Size.x;
	descDepth.Height = m_Size.y;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = Globals::pGpu->GetDevice().CreateTexture2D(&descDepth, nullptr, &pTempTexture);

	if (FAILED(hr))
	{
		std::cout << "depthStencil-Texture creation failed\n";
		return;
	}

	//VIEW
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	hr = Globals::pGpu->GetDevice().CreateDepthStencilView(pTempTexture, // Depth stencil texture
		&descDSV, // Depth stencil desc
		&m_pDepthStencilView);  // [out] Depth stencil view

	pTempTexture->Release();
}

void Rendering::Canvas::SetViewPort()
{
	m_ViewPort = {
	  0.0f, 0.0f,
	 static_cast<float>(m_Size.x),
	 static_cast<float>(m_Size.y),
	  0.0f, 1.0f };
	Globals::pGpu->GetContext().RSSetViewports(1, &m_ViewPort);
}

void Rendering::Canvas::GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter,
                                    IDXGIFactory2*& pFactory) const
{
	HRESULT hr = Globals::pGpu->GetDevice().QueryInterface(__uuidof(IDXGIDevice2), reinterpret_cast<void**>(&pDevice2));
	if (FAILED(hr))
		Logger::PrintError("Canvas::GetFactory2::Device2");

	hr = pDevice2->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pAdapter));
	if (FAILED(hr))
		Logger::PrintError("Canvas::GetFactory2::Adapter");

	hr = pAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pFactory));
	if (FAILED(hr))
		Logger::PrintError("Canvas::GetFactory2::Factory");
}
