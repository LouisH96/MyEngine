#pragma once
#include "d3d11.h"

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
			IDXGISwapChain* m_pSwapChain{};
		};
	}
}
