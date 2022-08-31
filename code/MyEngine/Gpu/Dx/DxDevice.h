#pragma once
#include <DirectXMath.h>
#include <dxgi1_2.h>
#include "d3d11.h"

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

				explicit DxDevice(App::Win32::Win32Window& window);
				~DxDevice() override;
				
				void Release() override;

				ICanvas* MakeCanvas() override;
				IMesh* MakeMesh() override;
				IShader* MakeShader() override;
				IPainter* MakePainter() override;

				ID3D11Device& GetDevice() const { return *m_pDevice; }
				ID3D11DeviceContext& GetContext() const { return *m_pContext; }

			private:
				struct Vertex
				{
					DirectX::XMFLOAT3 pos{};
					DirectX::XMFLOAT3 col{};
				};

				ID3D11Device* m_pDevice{};
				ID3D11DeviceContext* m_pContext{};
				App::Win32::Win32Window& m_Window;

				void Init();
			};
		}
	}
}
