#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

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
	namespace App
	{
		namespace Wrappers
		{
			namespace Gpu
			{
				class Gpu;

				class Shader
				{
				public:
					Shader(const Shader& other) = delete;
					Shader(Shader&& other) noexcept = delete;
					Shader& operator=(const Shader& other) = delete;
					Shader& operator=(Shader&& other) noexcept = delete;

					explicit Shader(Gpu& gpu);
					~Shader();

					void Activate() const;
					void OnCamUpdated(Game::Camera::Camera& camera);

					struct Vertex
					{
						DirectX::XMFLOAT3 pos{};
						DirectX::XMFLOAT3 col{};
					};

				private:

					Gpu& m_Gpu;

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
}

