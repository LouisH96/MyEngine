#pragma once
#include "d3d11.h"

namespace MyEngine
{
	namespace Framework
	{
		namespace Dx
		{
			class DxFramework
			{
			public:
				DxFramework(HWND windowHandle);
				~DxFramework();
				void Release();

			private:
				void Init(HWND windowHandle);

				ID3D11Device* m_pDevice{};
				ID3D11DeviceContext* m_pContext{};
				IDXGISwapChain* m_pSwapChain{};
			};
		}
	}
}
