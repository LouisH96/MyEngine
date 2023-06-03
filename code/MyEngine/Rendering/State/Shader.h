#pragma once
#include <d3d11.h>
#include "../Gpu.h"

namespace MyEngine
{
	namespace Rendering
	{
		class Shader
		{
		public:
			Shader(const Shader& other) = delete;
			Shader(Shader&& other) noexcept = delete;
			Shader& operator=(const Shader& other) = delete;
			Shader& operator=(Shader&& other) noexcept = delete;

			explicit Shader(const std::wstring& fullPath);
			explicit Shader(const Gpu& gpu, const std::wstring& fullPath);
			~Shader();

			void Activate() const;

		private:
			const Gpu& m_Gpu;

			ID3D11VertexShader* m_pVertexShader{};
			ID3D11PixelShader* m_pPixelShader{};

			void InitShaders(const std::wstring& fullPath);
		};
	}
}

