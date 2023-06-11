#pragma once
#include <dxgiformat.h>
#include <wincodec.h>

struct ID3D11ShaderResourceView;

namespace MyEngine
{
	namespace Rendering
	{
		class Image;
		class Gpu;
		class Texture
		{
		public:
			Texture(const Gpu& gpu, const std::wstring& path);
			Texture(const Gpu& gpu, Image* pImage);
			Texture(const std::wstring& path);
			Texture(Image& image);
			Texture(Image* pImage);
			Texture();
			~Texture();
			Texture(const Texture& other) = delete;
			Texture& operator=(const Texture& other) = delete;
			Texture(Texture&& other) noexcept;
			Texture& operator=(Texture&& other) noexcept;

			void ActivateVs(const Gpu& gpu) const;
			void ActivatePs(const Gpu& gpu) const;
			void Activate(const Gpu& gpu) const;

		private:
			ID3D11ShaderResourceView* m_pShaderResourceView{};

			static DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGuid);
			static WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
			static int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);
		};
	}
}
