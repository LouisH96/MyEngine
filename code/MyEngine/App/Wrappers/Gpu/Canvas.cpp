#include "pch.h"
#include "Canvas.h"

#include "DxHelper.h"
#include "App/Wrappers/Win32/Window.h"
#include "Gpu.h"

MyEngine::App::Wrappers::Gpu::Canvas::Canvas(Gpu& gpu, App::Wrappers::Win32::Window& window)
	: m_Gpu{ gpu }
{
	InitSwapChain(window);
	InitRenderTarget();
	InitDepthStencilState();
	InitDepthStencil(window.GetClientSize());
	SetViewPort(window.GetClientSize());
}

void MyEngine::App::Wrappers::Gpu::Canvas::Activate() const
{
	//DepthStencil
	m_Gpu.GetContext().OMSetDepthStencilState(m_pDepthStencilState, 1);

	//Rendertarget
	m_Gpu.GetContext().OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pDepthStencilView);
}

MyEngine::App::Wrappers::Gpu::Canvas::~Canvas()
{
	SAFE_RELEASE(m_pMainRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilState);
}

void MyEngine::App::Wrappers::Gpu::Canvas::Clear() const
{
	/* clear the back buffer to cornflower blue for the new frame */
	constexpr float background_colour[4] = {
	  0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
	m_Gpu.GetContext().ClearRenderTargetView(
		m_pMainRenderTargetView, background_colour);
	m_Gpu.GetContext().ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void MyEngine::App::Wrappers::Gpu::Canvas::Present() const
{
	DXGI_PRESENT_PARAMETERS param{ 0,nullptr,0,nullptr };
	m_pSwapChain->Present1(0, DXGI_PRESENT_DO_NOT_WAIT, &param);
}

void MyEngine::App::Wrappers::Gpu::Canvas::OnWindowResized(DirectX::XMINT2 newSize)
{
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pMainRenderTargetView);
	m_pSwapChain->ResizeBuffers(0, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0);
	InitRenderTarget();
	InitDepthStencil(newSize);
	SetViewPort(newSize);
}

void MyEngine::App::Wrappers::Gpu::Canvas::InitSwapChain(const App::Wrappers::Win32::Window& window)
{
	const DirectX::XMINT2 windowSize = window.GetClientSize();
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
		throw std::exception("Canvas::InitSwapChain");

	pFactory->Release();
	pAdapter->Release();
	pDevice2->Release();
}

void MyEngine::App::Wrappers::Gpu::Canvas::InitRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
		m_Gpu.GetDevice().CreateRenderTargetView(pBackBuffer, nullptr, &m_pMainRenderTargetView);
	pBackBuffer->Release();
}

void MyEngine::App::Wrappers::Gpu::Canvas::InitDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	m_Gpu.GetDevice().CreateDepthStencilState(&dsDesc, &m_pDepthStencilState);
}

void MyEngine::App::Wrappers::Gpu::Canvas::InitDepthStencil(const DirectX::XMINT2& size)
{
	//TEXTURE
	ID3D11Texture2D* pTempTexture;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = size.x;
	descDepth.Height = size.y;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = m_Gpu.GetDevice().CreateTexture2D(&descDepth, nullptr, &pTempTexture);

	//VIEW
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	hr = m_Gpu.GetDevice().CreateDepthStencilView(pTempTexture, // Depth stencil texture
		&descDSV, // Depth stencil desc
		&m_pDepthStencilView);  // [out] Depth stencil view

	pTempTexture->Release();
}

void MyEngine::App::Wrappers::Gpu::Canvas::SetViewPort(DirectX::XMINT2 size)
{
	m_ViewPort = {
	  0.0f, 0.0f,
	 static_cast<float>(size.x),
	 static_cast<float>(size.y),
	  0.0f, 1.0f };
	m_Gpu.GetContext().RSSetViewports(1, &m_ViewPort);
}

void MyEngine::App::Wrappers::Gpu::Canvas::GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter,
	IDXGIFactory2*& pFactory) const
{
	HRESULT hr = m_Gpu.GetDevice().QueryInterface(__uuidof(IDXGIDevice2), reinterpret_cast<void**>(&pDevice2));
	if (FAILED(hr))
		throw std::exception("Canvas::GetFactory2::Device2");

	hr = pDevice2->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pAdapter));
	if (FAILED(hr))
		throw std::exception("Canvas::GetFactory2::Adapter");

	hr = pAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pFactory));
	if (FAILED(hr))
		throw std::exception("Canvas::GetFactory2::Factory");
}
