#pragma once
#include "d3d11.h"
#include <dxgi1_2.h>

#include "../IGpu.h"

namespace MyEngine
{
	namespace Gpu
	{
		namespace DirectX
		{
			class DxDevice final
				: public IGpu
			{
			public:
				DxDevice(const DxDevice& other) = delete;
				DxDevice(DxDevice&& other) noexcept = delete;
				DxDevice& operator=(const DxDevice& other) = delete;
				DxDevice& operator=(DxDevice&& other) noexcept = delete;

				explicit DxDevice(HWND windowHandle);
				~DxDevice() override;
				
				void Release() override;

				void Temp() const override;

			private:
				void Init(HWND windowHandle);

				ID3D11Device* m_pDevice{};
				ID3D11DeviceContext* m_pContext{};
				IDXGISwapChain1* m_pSwapChain{};

				//temp
				void TempInit(HWND windowHandle);
				void TempRender() const;
				ID3D11VertexShader* m_pVertexShader{};
				ID3D11PixelShader* m_pPixelShader{};
				ID3D11InputLayout* m_pInputLayout{};
				ID3D11Buffer* m_pVertexBuffer{};
				ID3D11RenderTargetView* m_pMainRenderTargetView{};
			};
		}
	}
}
