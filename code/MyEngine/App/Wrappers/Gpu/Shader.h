#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Gpu.h"
#include "DxHelper.h"
#include "App/Resources.h"
#include "Game/Camera/Camera.h"

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Gpu
			{
				template<typename Vertex>
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

				private:

					Gpu& m_Gpu;

					ID3D11VertexShader* m_pVertexShader{};
					ID3D11PixelShader* m_pPixelShader{};

					void InitShaders();

					struct CBuffer
					{
						DirectX::XMFLOAT4X4 cameraMatrix;
					};
					ID3D11Buffer* m_pCBuffer{};
					void InitCBuffer();
					void ReleaseCBuffer();
					void UpdateCBuffer(const Game::Camera::Camera& camera) const;
				};

				template <typename Vertex>
				Shader<Vertex>::Shader(Gpu& gpu)
					: m_Gpu(gpu)
				{
					InitShaders();
					InitCBuffer();
				}

				template <typename Vertex>
				Shader<Vertex>::~Shader()
				{
					SAFE_RELEASE(m_pPixelShader);
					SAFE_RELEASE(m_pVertexShader);
					ReleaseCBuffer();
				}

				template <typename Vertex>
				void Shader<Vertex>::Activate() const
				{
					m_Gpu.GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					m_Gpu.GetContext().VSSetShader(m_pVertexShader, nullptr, 0);
					m_Gpu.GetContext().PSSetShader(m_pPixelShader, nullptr, 0);
					m_Gpu.GetContext().VSSetConstantBuffers(0, 1, &m_pCBuffer);
				}

				template <typename Vertex>
				void Shader<Vertex>::OnCamUpdated(Game::Camera::Camera& camera)
				{
					UpdateCBuffer(*&camera);
				}

				template <typename Vertex>
				void Shader<Vertex>::InitShaders()
				{
					const std::wstring path = Resources::GetShaderPath(L"shader.hlsl");
					DxHelper::CreateVertexShader(m_Gpu.GetDevice(), path, "vs_main", m_pVertexShader);
					DxHelper::CreatePixelShader(m_Gpu.GetDevice(), path, "ps_main", m_pPixelShader);
				}

				template <typename Vertex>
				void Shader<Vertex>::InitCBuffer()
				{
					DxHelper::CreateDynamicConstantBuffer<CBuffer>(m_Gpu.GetDevice(), m_pCBuffer);
				}

				template <typename Vertex>
				void Shader<Vertex>::ReleaseCBuffer()
				{
					SAFE_RELEASE(m_pCBuffer);
				}

				template <typename Vertex>
				void Shader<Vertex>::UpdateCBuffer(const Game::Camera::Camera& camera) const
				{
					DxHelper::UpdateBuffer(m_Gpu.GetContext(), *m_pCBuffer, camera.GetViewProjMatrix());
				}
			}
		}
	}
}

