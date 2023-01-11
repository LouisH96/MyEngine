#pragma once

#include "../ICanvas.h"
#include "DxGpu.h"
#include <DirectXMath.h>

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Window;
		}
	}
}

namespace MyEngine
{
	namespace Gpu
	{
		namespace Dx
		{
			class DxCanvas final
				: public ICanvas
			{
			public:
				DxCanvas(const DxCanvas& other) = delete;
				DxCanvas(DxCanvas&& other) noexcept = delete;
				DxCanvas& operator=(const DxCanvas& other) = delete;
				DxCanvas& operator=(DxCanvas&& other) noexcept = delete;

				DxCanvas(DxGpu& gpu, App::Win32::Window& window);
				~DxCanvas() override;

				void Clear() const override;
				void Present() const override;
				void Activate() const;

				void OnWindowResized(DirectX::XMINT2 newSize) override;

			private:
				DxGpu& m_Gpu;
				IDXGISwapChain1* m_pSwapChain{};
				ID3D11RenderTargetView* m_pMainRenderTargetView{};
				D3D11_VIEWPORT m_ViewPort{};

				void InitSwapChain(const App::Win32::Window& window);
				void InitRenderTarget();
				void SetViewPort(DirectX::XMINT2 windowSize);

				void GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter, IDXGIFactory2*& pFactory) const;//clean after use
			};
		}
	}
}
