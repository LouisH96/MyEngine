#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../IShader.h"

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class Camera;
		}
	}
}

namespace MyEngine
{
	namespace Gpu
	{
		namespace Dx
		{
			class DxGpu;

			class DxShader final
				: public IShader
			{
			public:
				DxShader(const DxShader& other) = delete;
				DxShader(DxShader&& other) noexcept = delete;
				DxShader& operator=(const DxShader& other) = delete;
				DxShader& operator=(DxShader&& other) noexcept = delete;

				explicit DxShader(DxGpu& gpu);
				~DxShader() override;

				void Activate() const;
				void OnCamUpdated(Game::Camera::ICamera& camera) override;

				struct Vertex
				{
					DirectX::XMFLOAT3 pos{};
					DirectX::XMFLOAT3 col{};
				};

			private:

				DxGpu& m_Gpu;

				ID3D11VertexShader* m_pVertexShader{};
				ID3D11PixelShader* m_pPixelShader{};
				ID3D11InputLayout* m_pInputLayout{};

				void InitShaders();
				void InitInputLayout();

#pragma region CBuffer

			private:
				struct CBuffer
				{
					DirectX::XMFLOAT4X4 cameraMatrix;
				};
				ID3D11Buffer* m_pCBuffer{};
				void InitCBuffer();
				void ReleaseCBuffer();
				void UpdateCBuffer(const Game::Camera::Camera& camera) const;

#pragma endregion

			};
		}
	}
}

