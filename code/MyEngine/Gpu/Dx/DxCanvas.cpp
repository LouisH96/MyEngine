#include "DxCanvas.h"

#include "DxHelper.h"
#include "../../App/Win32/Win32Window.h"

MyEngine::Gpu::Dx::DxCanvas::DxCanvas(ID3D11Device& device, ID3D11DeviceContext& context, App::Win32::Win32Window& window)
	: m_Device{ device }
	, m_Context{ context }
{
	InitSwapChain(window);
	InitRenderTarget();
	SetViewPort(window.GetSize());

	window.Listen(*this);
}

MyEngine::Gpu::Dx::DxCanvas::~DxCanvas()
{
	SAFE_RELEASE(m_pMainRenderTargetView)
		SAFE_RELEASE(m_pSwapChain)
}

void MyEngine::Gpu::Dx::DxCanvas::BeginPaint() const
{
	/* clear the back buffer to cornflower blue for the new frame */
	constexpr float background_colour[4] = {
	  0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
	m_Context.ClearRenderTargetView(
		m_pMainRenderTargetView, background_colour);

	m_Context.OMSetRenderTargets(1, &m_pMainRenderTargetView, nullptr);
}

void MyEngine::Gpu::Dx::DxCanvas::Render() const
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

void MyEngine::Gpu::Dx::DxCanvas::InitSwapChain(const App::Win32::Win32Window& window)
{
	const DirectX::XMINT2 windowSize = window.GetSize();
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

	if (pFactory->CreateSwapChainForHwnd(&m_Device, window.GetWindowHandle(), &desc, nullptr, nullptr, &m_pSwapChain) != S_OK)
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
		m_Device.CreateRenderTargetView(pBackBuffer, nullptr, &m_pMainRenderTargetView);
	pBackBuffer->Release();
}

void MyEngine::Gpu::Dx::DxCanvas::SetViewPort(DirectX::XMINT2 windowSize)
{
	m_ViewPort = {
	  0.0f, 0.0f,
	 static_cast<float>(windowSize.x),
	 static_cast<float>(windowSize.y),
	  0.0f, 1.0f };
	m_Context.RSSetViewports(1, &m_ViewPort);
}

void MyEngine::Gpu::Dx::DxCanvas::GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter,
	IDXGIFactory2*& pFactory) const
{
	HRESULT hr = m_Device.QueryInterface(__uuidof(IDXGIDevice2), reinterpret_cast<void**>(&pDevice2));
	if (FAILED(hr))
		throw std::exception("DxCanvas::GetFactory2::Device2");

	hr = pDevice2->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pAdapter));
	if (FAILED(hr))
		throw std::exception("DxCanvas::GetFactory2::Adapter");

	hr = pAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pFactory));
	if (FAILED(hr))
		throw std::exception("DxCanvas::GetFactory2::Factory");
}
