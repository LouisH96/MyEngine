#pragma once

namespace MyEngine
{
	namespace Rendering
	{
		class PixelShader
		{
		public:
			explicit PixelShader(const std::wstring& path, const std::string& functionName = "ps_main");

			PixelShader(const PixelShader& other) = delete;
			PixelShader(PixelShader&& other) noexcept;
			PixelShader& operator=(const PixelShader& other) = delete;
			PixelShader& operator=(PixelShader&& other) noexcept;

			~PixelShader();

			void Activate() const;

		private:
			ID3D11PixelShader* m_pShader;

		};
	}
}
