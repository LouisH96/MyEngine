#pragma once
#include <d3d11.h>

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
		class Gpu;

		class Canvas
		{
		public:
			Canvas(const Canvas& other) = delete;
			Canvas(Canvas&& other) noexcept = delete;
			Canvas& operator=(const Canvas& other) = delete;
			Canvas& operator=(Canvas&& other) noexcept = delete;

			Canvas(App::Win32::Window& window);
			~Canvas();

			void BeginPaint() const;
			void Present() const;

			void OnWindowResized(Math::Int2 newSize);
			const Int2& GetSize() const { return m_Size; }
			float GetAspectRatio() const { return Float::Cast(m_Size.x) / m_Size.y; }

		private:
			IDXGISwapChain1* m_pSwapChain{};
			ID3D11RenderTargetView* m_pMainRenderTargetView{};
			D3D11_VIEWPORT m_ViewPort{};

			ID3D11DepthStencilView* m_pDepthStencilView{};

			Int2 m_Size;

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

