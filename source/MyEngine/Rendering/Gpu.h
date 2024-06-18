#pragma once
#include "d3d11.h"
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

	namespace Rendering
	{
		class Canvas;

		class Gpu
		{
		public:
			Gpu(const Gpu& other) = delete;
			Gpu(Gpu&& other) noexcept = delete;
			Gpu& operator=(const Gpu& other) = delete;
			Gpu& operator=(Gpu&& other) noexcept = delete;

			explicit Gpu(App::Win32::Window& window);
			~Gpu();

			void Release();

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
