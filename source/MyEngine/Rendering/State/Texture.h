#pragma once
#include <dxgiformat.h>
#include <wincodec.h>

struct ID3D11Texture2D;
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
	Texture(Image* pImage, bool dynamic = false);
	Texture(Image& image, bool dynamic = false);
	Texture(ID3D11ShaderResourceView* pResourceView);
	Texture() = default;
	~Texture();
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void ActivateVs(unsigned slot = 0) const;
	void ActivatePs(unsigned slot = 0) const;
	void Activate(unsigned slot = 0) const;

	void Update(const Image& image);

	D3D11_TEXTURE2D_DESC GetDesc() const;

	static void Unset(unsigned slot);

private:
	ID3D11Texture2D* m_pTexture{};
	ID3D11ShaderResourceView* m_pShaderResourceView{};

	static DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGuid);
	static WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
	static int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);
};
}
}
