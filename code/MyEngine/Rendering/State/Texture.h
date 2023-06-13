#pragma once
#include <dxgiformat.h>
#include <wincodec.h>

struct ID3D11ShaderResourceView;

namespace MyEngine
{
	namespace Rendering
	{
		class Image;
		class Texture
		{
		public:
			Texture(const std::wstring& path);
			Texture(Image* pImage);
			Texture(Image& image);
			Texture();
			~Texture();
			Texture(const Texture& other) = delete;
			Texture& operator=(const Texture& other) = delete;
			Texture(Texture&& other) noexcept;
			Texture& operator=(Texture&& other) noexcept;

			void ActivateVs() const;
			void ActivatePs() const;
			void Activate() const;

		private:
			ID3D11ShaderResourceView* m_pShaderResourceView{};

			static DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGuid);
			static WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
			static int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);
		};
	}
}
