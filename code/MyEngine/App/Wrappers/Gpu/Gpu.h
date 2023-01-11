#pragma once
#include <DirectXMath.h>
#include <dxgi1_2.h>
#include "d3d11.h"

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
				class Canvas;
				class Mesh;
				class Shader;
				class Painter;
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
				class Gpu
				{
				public:
					Gpu(const Gpu& other) = delete;
					Gpu(Gpu&& other) noexcept = delete;
					Gpu& operator=(const Gpu& other) = delete;
					Gpu& operator=(Gpu&& other) noexcept = delete;

					explicit Gpu(Win32::Window& window);
					~Gpu();

					void Release();

					Canvas* MakeCanvas();
					Mesh* MakeMesh();
					Shader* MakeShader();
					Painter* MakePainter();

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
					Win32::Window& m_Window;

					void Init();
				};
			}
		}
	}
}
