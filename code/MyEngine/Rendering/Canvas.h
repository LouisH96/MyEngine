#pragma once
#include <d3d11.h>

#include "App/ResizedEvent.h"

struct IDXGISwapChain1;
struct IDXGIDevice2;
struct IDXGIFactory2;

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Window;
		}
	}

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

			void BeginPaint() const;
			void Present() const;

			App::ResizedEvent OnWindowResized(Int2 newSize);
			const Int2& GetSize() const { return m_Size; }
			float GetWidthF() const { return static_cast<float>(m_Size.x); }
			float GetHeightF() const { return static_cast<float>(m_Size.y); }
			float GetAspectRatio() const { return Float::Cast(m_Size.x) / m_Size.y; }
			float GetInvAspectRatio() const { return static_cast<float>(m_Size.y) / static_cast<float>(m_Size.x); }

		private:
			IDXGISwapChain1* m_pSwapChain{};
			ID3D11RenderTargetView* m_pMainRenderTargetView{};
			D3D11_VIEWPORT m_ViewPort{};

			ID3D11DepthStencilView* m_pDepthStencilView{};

			Int2 m_Size;
			Float4 m_Color;

			void Clear() const;
			void Activate() const;

			void InitSwapChain(const App::Win32::Window& window);
			void InitRenderTarget();
			void InitDepthStencil();
			void SetViewPort();

			void GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter, IDXGIFactory2*& pFactory) const;//clean after use
		};
	}
}

