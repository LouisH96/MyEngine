#pragma once
#include "d3d11.h"
#include <dxgi1_2.h>

namespace MyEngine
{
	namespace DirectX
	{
		class DxDevice
		{
		public:
			DxDevice(HWND windowHandle);
			~DxDevice();
			void Release();

			void Render() const;

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
