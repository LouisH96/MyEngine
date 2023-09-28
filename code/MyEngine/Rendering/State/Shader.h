#pragma once
#include <d3d11.h>

namespace MyEngine
{
	namespace Rendering
	{
		class Shader
		{
		public:
			Shader(const Shader& other) = delete;
			Shader(Shader&& other) noexcept;
			Shader& operator=(const Shader& other) = delete;
			Shader& operator=(Shader&& other) noexcept;

			explicit Shader(const std::wstring& fullPath);
			~Shader();

			void Activate() const;

		private:
			ID3D11VertexShader* m_pVertexShader{};
			ID3D11PixelShader* m_pPixelShader{};

			void InitShaders(const std::wstring& fullPath);
		};
	}
}

