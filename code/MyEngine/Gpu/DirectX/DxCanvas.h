#pragma once

#include "../ICanvas.h"
#include "DxDevice.h"
#include <DirectXMath.h>

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Win32Window;
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

				DxCanvas(ID3D11Device& device, ID3D11DeviceContext& context, App::Win32::Win32Window& window);
				~DxCanvas() override;

				void BeginPaint() const override;
				void Render() const override;
				void OnWindowResized(DirectX::XMINT2 newSize) override;

			private:
				ID3D11Device& m_Device;
				ID3D11DeviceContext& m_Context;
				IDXGISwapChain1* m_pSwapChain{};
				ID3D11RenderTargetView* m_pMainRenderTargetView{};
				D3D11_VIEWPORT m_ViewPort{};

				void InitSwapChain(const App::Win32::Win32Window& window);
				void InitRenderTarget();
				void SetViewPort(DirectX::XMINT2 windowSize);

				void GetFactory2(IDXGIDevice2*& pDevice2, IDXGIAdapter*& pAdapter, IDXGIFactory2*& pFactory) const;//clean after use
			};
		}
	}
}
