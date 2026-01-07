#pragma once

#include "State\DepthStencilBuffer.h"
#include "State\Viewport.h"
#include <App\ResizedEvent.h>

#include <d3d11.h>

struct IDXGIDevice2;
struct IDXGIFactory2;
struct IDXGISwapChain1;

namespace MyEngine
{
//Forward Declarations
namespace App {
namespace Win32 {
class Window;
}
}

//Class
namespace Rendering
{
class Canvas
{
public:
	//---| Construction |---
	Canvas(const Canvas& other) = delete;
	Canvas(Canvas&& other) noexcept = delete;
	Canvas& operator=(const Canvas& other) = delete;
	Canvas& operator=(Canvas&& other) noexcept = delete;

	explicit Canvas(App::Win32::Window& window, const Float3& color);
	~Canvas();

	void ClearDepthBuffer();
	void BeginPaint();
	void Present() const;

	App::ResizedEvent OnWindowResized(Int2 newSize);
	const Int2& GetSize() const { return m_Size; }
	float GetWidthF() const { return static_cast<float>(m_Size.x); }
	float GetHeightF() const { return static_cast<float>(m_Size.y); }
	float GetAspectRatio() const { return Float::Cast(m_Size.x) / m_Size.y; }
	float GetInvAspectRatio() const { return static_cast<float>(m_Size.y) / static_cast<float>(m_Size.x); }
	const Float2& GetInvSize() const { return m_InvSize; }

private:
	static constexpr DXGI_FORMAT SWAPCHAIN_FORMAT{ DXGI_FORMAT_R8G8B8A8_UNORM };
	IDXGISwapChain1* m_pSwapChain{};
	ID3D11Texture2D* m_pSwapChainBuffer{};

	ID3D11RenderTargetView* m_pRenderTargetView{};
	ID3D11Texture2D* m_pRenderTargetTexture{};
	
	Viewport m_Viewport;
	DepthStencilBuffer m_DepthStencilBuffer;

	Int2 m_Size;
	Float2 m_InvSize;
	Float4 m_Color;

	void Clear();
	void Activate();

	void InitSwapChain(const App::Win32::Window& window);
	void InitRenderTarget();

	void GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter, IDXGIFactory2*& pFactory) const;//clean after use
};

}
}
