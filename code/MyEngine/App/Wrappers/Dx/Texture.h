#pragma once
#include <dxgiformat.h>
#include <wincodec.h>

struct ID3D11ShaderResourceView;

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Dx
			{
				class Gpu;
			}
		}
	}
}

namespace MyEngine
{
	namespace Dx
	{
		class Texture
		{
		public:
			Texture(const App::Wrappers::Dx::Gpu& gpu, const std::wstring& path);
			~Texture();
			Texture(const Texture& other) = delete;
			Texture(Texture&& other) noexcept = delete;
			Texture& operator=(const Texture& other) = delete;
			Texture& operator=(Texture&& other) noexcept = delete;

			void ActivateVs(const App::Wrappers::Dx::Gpu& gpu) const;
			void ActivatePs(const App::Wrappers::Dx::Gpu& gpu) const;
			void Activate(const App::Wrappers::Dx::Gpu& gpu) const;

		private:
			ID3D11ShaderResourceView* m_pShaderResourceView;

			static DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGuid);
			static WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
			static int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);
		};
	}
}
