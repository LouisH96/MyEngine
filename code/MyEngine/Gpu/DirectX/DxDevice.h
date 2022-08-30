#pragma once
#include "d3d11.h"
#include <dxgi1_2.h>

#include "../IGpu.h"

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

				ICanvas* MakeCanvas(App::IWindow& window) override;
				void Temp() const override;

			private:
				ID3D11Device* m_pDevice{};
				ID3D11DeviceContext* m_pContext{};

				void Init(HWND windowHandle);

				//temp
				void TempInit(HWND windowHandle);
				void TempRender() const;

			private:
				ID3D11VertexShader* m_pVertexShader{};
				ID3D11PixelShader* m_pPixelShader{};
				ID3D11InputLayout* m_pInputLayout{};
				ID3D11Buffer* m_pVertexBuffer{};
			};
		}
	}
}
