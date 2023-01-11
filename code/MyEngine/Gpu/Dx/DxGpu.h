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
			class DxGpu final
				: public IGpu
			{
			public:
				DxGpu(const DxGpu& other) = delete;
				DxGpu(DxGpu&& other) noexcept = delete;
				DxGpu& operator=(const DxGpu& other) = delete;
				DxGpu& operator=(DxGpu&& other) noexcept = delete;

				explicit DxGpu(App::Win32::Window& window);
				~DxGpu() override;

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
				App::Win32::Window& m_Window;

				void Init();
			};
		}
	}
}
