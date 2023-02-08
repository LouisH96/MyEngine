#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Gpu.h"
#include "DxHelper.h"

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Dx
			{
				template<typename Vertex>
				class Shader
				{
				public:
					Shader(const Shader& other) = delete;
					Shader(Shader&& other) noexcept = delete;
					Shader& operator=(const Shader& other) = delete;
					Shader& operator=(Shader&& other) noexcept = delete;

					explicit Shader(Gpu& gpu, const std::wstring& fullPath);
					~Shader();

					void Activate() const;

				private:
					Gpu& m_Gpu;

					ID3D11VertexShader* m_pVertexShader{};
					ID3D11PixelShader* m_pPixelShader{};

					void InitShaders(const std::wstring& fullPath);
				};

				template <typename Vertex>
				Shader<Vertex>::Shader(Gpu& gpu, const std::wstring& fullPath)
					: m_Gpu(gpu)
				{
					InitShaders(fullPath);
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
				void Shader<Vertex>::InitShaders(const std::wstring& fullPath)
				{
					DxHelper::CreateVertexShader(m_Gpu.GetDevice(), fullPath, "vs_main", m_pVertexShader);
					DxHelper::CreatePixelShader(m_Gpu.GetDevice(), fullPath, "ps_main", m_pPixelShader);
				}
			}
		}
	}
}

