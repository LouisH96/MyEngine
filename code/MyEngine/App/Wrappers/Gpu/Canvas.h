#pragma once

#include "Gpu.h"
#include <DirectXMath.h>

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Win32
			{
				class Window;
			}
			namespace Gpu
			{
				class Gpu;
			}
		}
	}
}

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Gpu
			{
				class Canvas
				{
				public:
					Canvas(const Canvas& other) = delete;
					Canvas(Canvas&& other) noexcept = delete;
					Canvas& operator=(const Canvas& other) = delete;
					Canvas& operator=(Canvas&& other) noexcept = delete;

					Canvas(Gpu& gpu, Wrappers::Win32::Window& window);
					~Canvas();

					void Clear() const;
					void Present() const;
					void Activate() const;

					void OnWindowResized(DirectX::XMINT2 newSize);

				private:
					Gpu& m_Gpu;
					IDXGISwapChain1* m_pSwapChain{};
					ID3D11RenderTargetView* m_pMainRenderTargetView{};
					D3D11_VIEWPORT m_ViewPort{};

					ID3D11Texture2D* m_pDepthStencil{};
					ID3D11DepthStencilState* m_pDepthStencilState{};
					ID3D11DepthStencilView* m_pDepthStencilView{};

					void InitSwapChain(const Win32::Window& window);
					void InitRenderTarget();
					void InitDepthStencil(const DirectX::XMINT2& size);
					void SetViewPort(DirectX::XMINT2 size);

					void GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter, IDXGIFactory2*& pFactory) const;//clean after use
				};
			}
		}
	}
}

