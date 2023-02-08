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

				private:
					Gpu& m_Gpu;

					ID3D11VertexShader* m_pVertexShader{};
					ID3D11PixelShader* m_pPixelShader{};

					void InitShaders();
				};

				template <typename Vertex>
				Shader<Vertex>::Shader(Gpu& gpu)
					: m_Gpu(gpu)
				{
					InitShaders();
				}

				template <typename Vertex>
				Shader<Vertex>::~Shader()
				{
					SAFE_RELEASE(m_pPixelShader);
					SAFE_RELEASE(m_pVertexShader);
				}

				template <typename Vertex>
				void Shader<Vertex>::Activate() const
				{
					m_Gpu.GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					m_Gpu.GetContext().VSSetShader(m_pVertexShader, nullptr, 0);
					m_Gpu.GetContext().PSSetShader(m_pPixelShader, nullptr, 0);
				}

				template <typename Vertex>
				void Shader<Vertex>::InitShaders()
				{
					const std::wstring path = Resources::GetShaderPath(L"shader.hlsl");
					DxHelper::CreateVertexShader(m_Gpu.GetDevice(), path, "vs_main", m_pVertexShader);
					DxHelper::CreatePixelShader(m_Gpu.GetDevice(), path, "ps_main", m_pPixelShader);
				}
			}
		}
	}
}

