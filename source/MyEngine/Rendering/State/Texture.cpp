#include "pch.h"
#include "Texture.h"

#include <wincodec.h>
#include <Rendering/Gpu.h>
#include <Image/Image.h>

using namespace Rendering;

Texture::Texture(const std::wstring& path)
{
	//https://www.braynzarsoft.net/viewtutorial/q16390-directx-12-textures-from-file
	static IWICImagingFactory* wicFactory;
	HRESULT hr;
	IWICBitmapDecoder* pWicDecoder{};
	IWICBitmapFrameDecode* pWicFrame{};
	IWICFormatConverter* pWicConverter{};
	bool imageConverted = false;

	if (wicFactory == nullptr)
	{
		CoInitialize(nullptr);
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory),
			reinterpret_cast<LPVOID*>(&wicFactory));
		if (FAILED(hr))
			Logger::Error("Failed creating wicFactory");
	}

	hr = wicFactory->CreateDecoderFromFilename(
		path.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pWicDecoder);

	if (FAILED(hr))
		Logger::Error("Failed creating decoder");

	hr = pWicDecoder->GetFrame(0, &pWicFrame);
	if (FAILED(hr))
		Logger::Error("Failed getting first frame");

	WICPixelFormatGUID pixelFormat;
	hr = pWicFrame->GetPixelFormat(&pixelFormat);
	if (FAILED(hr))
		Logger::Error("Failed getting pixel format");

	unsigned textureWidth, textureHeight;
	hr = pWicFrame->GetSize(&textureWidth, &textureHeight);
	if (FAILED(hr))
		Logger::Error("Failed getting texture size");

	DXGI_FORMAT dxgiFormat = GetDXGIFormatFromWICFormat(pixelFormat);

	// if the format of the image is not a supported dxgi format, try to convert it
	if (dxgiFormat == DXGI_FORMAT_UNKNOWN)
	{
		//get a dxgi compatible wic format from the current image format
		WICPixelFormatGUID convertToPixelFormat = GetConvertToWICFormat(pixelFormat);

		//return if no dxgi compatible format was found
		if (convertToPixelFormat == GUID_WICPixelFormatDontCare)
		{
			Logger::Error("No dxgi compatible format was found");
			return;
		}

		//set the dxgi format
		dxgiFormat = GetDXGIFormatFromWICFormat(convertToPixelFormat);

		//create the format converter
		hr = wicFactory->CreateFormatConverter(&pWicConverter);
		if (FAILED(hr))
		{
			Logger::Error("Failed creating format converter");
			return;
		}

		//make sure we can convert to the dxgi compatible format
		BOOL canConvert = false;
		hr = pWicConverter->CanConvert(pixelFormat, convertToPixelFormat, &canConvert);
		if (FAILED(hr) || !canConvert)
		{
			Logger::Error("Cannot convert");
			return;
		}

		//do the conversion (wicConverter will contain the converted image)
		hr = pWicConverter->Initialize(pWicFrame, convertToPixelFormat, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
		if (FAILED(hr))
		{
			Logger::Error("Failed initializing wicConverter");
			return;
		}
		//this is so we know to get the image data from the wicConverter (otherwise we will get from wicFrame)
		imageConverted = true;
	}

	const int bitsPerPixel = GetDXGIFormatBitsPerPixel(dxgiFormat);
	const int bytesPerRow = (textureWidth * bitsPerPixel) / 8;
	const int imageSize = bytesPerRow * textureHeight;

	//allocate enough memory for the raw image data, and set imageData to point to that memory
	uint8_t* pImageData = new uint8_t[imageSize];

	//copy (decoded) raw image data into the newly allocated memory (imageData)
	if (imageConverted)
	{
		//if image format needed to be converted, the wic converter will contain the converted image
		hr = pWicConverter->CopyPixels(nullptr, bytesPerRow, imageSize, pImageData);
		if (FAILED(hr))
		{
			Logger::Error("Failed copying pixels from converter");
			return;
		}
	}
	else
	{
		//no need to convert, just copy data from wic frame
		hr = pWicFrame->CopyPixels(nullptr, bytesPerRow, imageSize, pImageData);
		if (FAILED(hr))
		{
			Logger::Error("Failed copying pixels from frame");
			return;
		}
	}

	//fill out texture description
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = textureWidth;
	desc.Height = textureHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = dxgiFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = pImageData;
	initData.SysMemPitch = bytesPerRow;
	//initData.SysMemSlicePitch = bytesPerRow * textureHeight;

	ID3D11Texture2D* pTexture{};
	hr = Globals::pGpu->GetDevice().CreateTexture2D(&desc, &initData, &pTexture);
	if (FAILED(hr))
	{
		Logger::Error("Failed creating Texture2D");
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	Globals::pGpu->GetDevice().CreateShaderResourceView(pTexture, &srvDesc, &m_pShaderResourceView);
	if (FAILED(hr))
	{
		Logger::Error("Failed to create shaderResource");
		return;
	}

	pTexture->Release();
	delete[] pImageData;
}

Texture::Texture(Image* pImage, bool dynamic)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = pImage->GetWidth();
	desc.Height = pImage->GetHeight();
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0u;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.SysMemPitch = pImage->GetBytesPerRow();
	initData.pSysMem = pImage->GetData();

	HRESULT hr = Globals::pGpu->GetDevice().CreateTexture2D(&desc, &initData, &m_pTexture);
	if (FAILED(hr))
	{
		Logger::Error("Failed creating Texture2D");
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = Globals::pGpu->GetDevice().CreateShaderResourceView(m_pTexture, &srvDesc, &m_pShaderResourceView);
	if (FAILED(hr))
	{
		Logger::Error("Failed to create shaderResource");
		return;
	}
}

Texture::Texture(Image& image, bool dynamic)
	: Texture{ &image, dynamic }
{
}

Texture::Texture(ID3D11ShaderResourceView* pResourceView)
	: m_pShaderResourceView{ pResourceView }
	, m_pTexture{ nullptr }
{
}

Texture::~Texture()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pShaderResourceView) m_pShaderResourceView->Release();
	m_pShaderResourceView = nullptr;
	m_pTexture = nullptr;
}

Texture::Texture(Texture&& other) noexcept
	: m_pTexture{ other.m_pTexture }
	, m_pShaderResourceView{ other.m_pShaderResourceView }
{
	other.m_pTexture = nullptr;
	other.m_pShaderResourceView = nullptr;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (&other == this) return *this;
	if (m_pTexture) m_pTexture->Release();
	if (m_pShaderResourceView) m_pShaderResourceView->Release();
	m_pTexture = other.m_pTexture;
	m_pShaderResourceView = other.m_pShaderResourceView;
	other.m_pTexture = nullptr;
	other.m_pShaderResourceView = nullptr;
	return *this;
}

void Texture::ActivateVs(unsigned slot) const
{
	Globals::pGpu->GetContext().VSSetShaderResources(slot, 1, &m_pShaderResourceView);
}

void Texture::ActivatePs(unsigned slot) const
{
	Globals::pGpu->GetContext().PSSetShaderResources(slot, 1, &m_pShaderResourceView);
}

void Texture::Activate(unsigned slot) const
{
	Globals::pGpu->GetContext().PSSetShaderResources(slot, 1, &m_pShaderResourceView);
	Globals::pGpu->GetContext().VSSetShaderResources(slot, 1, &m_pShaderResourceView);
}

void Texture::Update(const Image& image)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	const HRESULT result{ Globals::pGpu->GetContext().Map(m_pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
	if (FAILED(result)) Logger::Error("Failed mapping texture");
	memcpy(mappedResource.pData, image.GetData(), image.GetBytesPerRow() * image.GetHeight());
	Globals::pGpu->GetContext().Unmap(m_pTexture, 0);
}

void Texture::Unset(unsigned slot)
{
	ID3D11ShaderResourceView* view[]{
		nullptr
	};
	Globals::pGpu->GetContext().VSSetShaderResources(slot, 1, view);
	Globals::pGpu->GetContext().PSSetShaderResources(slot, 1, view);
}

DXGI_FORMAT Texture::GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGuid)
{
	if (wicFormatGuid == GUID_WICPixelFormat128bppRGBAFloat) return DXGI_FORMAT_R32G32B32A32_FLOAT;
	if (wicFormatGuid == GUID_WICPixelFormat64bppRGBAHalf) return DXGI_FORMAT_R16G16B16A16_FLOAT;
	if (wicFormatGuid == GUID_WICPixelFormat64bppRGBA) return DXGI_FORMAT_R16G16B16A16_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat32bppRGBA) return DXGI_FORMAT_R8G8B8A8_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat32bppBGRA) return DXGI_FORMAT_B8G8R8A8_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat32bppBGR) return DXGI_FORMAT_B8G8R8X8_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat32bppRGBA1010102XR) return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat32bppRGBA1010102) return DXGI_FORMAT_R10G10B10A2_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat16bppBGRA5551) return DXGI_FORMAT_B5G5R5A1_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat16bppBGR565) return DXGI_FORMAT_B5G6R5_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat32bppGrayFloat) return DXGI_FORMAT_R32_FLOAT;
	if (wicFormatGuid == GUID_WICPixelFormat16bppGrayHalf) return DXGI_FORMAT_R16_FLOAT;
	if (wicFormatGuid == GUID_WICPixelFormat16bppGray) return DXGI_FORMAT_R16_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat8bppGray) return DXGI_FORMAT_R8_UNORM;
	if (wicFormatGuid == GUID_WICPixelFormat8bppAlpha) return DXGI_FORMAT_A8_UNORM;
	return DXGI_FORMAT_UNKNOWN;
}

WICPixelFormatGUID Texture::GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID)
{
	if (wicFormatGUID == GUID_WICPixelFormatBlackWhite) return GUID_WICPixelFormat8bppGray;
	if (wicFormatGUID == GUID_WICPixelFormat1bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat2bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat4bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat8bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat2bppGray) return GUID_WICPixelFormat8bppGray;
	if (wicFormatGUID == GUID_WICPixelFormat4bppGray) return GUID_WICPixelFormat8bppGray;
	if (wicFormatGUID == GUID_WICPixelFormat16bppGrayFixedPoint) return GUID_WICPixelFormat16bppGrayHalf;
	if (wicFormatGUID == GUID_WICPixelFormat32bppGrayFixedPoint) return GUID_WICPixelFormat32bppGrayFloat;
	if (wicFormatGUID == GUID_WICPixelFormat16bppBGR555) return GUID_WICPixelFormat16bppBGRA5551;
	if (wicFormatGUID == GUID_WICPixelFormat32bppBGR101010) return GUID_WICPixelFormat32bppRGBA1010102;
	if (wicFormatGUID == GUID_WICPixelFormat24bppBGR) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat24bppRGB) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat32bppPBGRA) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat32bppPRGBA) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat48bppRGB) return GUID_WICPixelFormat64bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat48bppBGR) return GUID_WICPixelFormat64bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat64bppBGRA) return GUID_WICPixelFormat64bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat64bppPRGBA) return GUID_WICPixelFormat64bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat64bppPBGRA) return GUID_WICPixelFormat64bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat48bppRGBFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	if (wicFormatGUID == GUID_WICPixelFormat48bppBGRFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	if (wicFormatGUID == GUID_WICPixelFormat64bppRGBAFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	if (wicFormatGUID == GUID_WICPixelFormat64bppBGRAFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	if (wicFormatGUID == GUID_WICPixelFormat64bppRGBFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	if (wicFormatGUID == GUID_WICPixelFormat64bppRGBHalf) return GUID_WICPixelFormat64bppRGBAHalf;
	if (wicFormatGUID == GUID_WICPixelFormat48bppRGBHalf) return GUID_WICPixelFormat64bppRGBAHalf;
	if (wicFormatGUID == GUID_WICPixelFormat128bppPRGBAFloat) return GUID_WICPixelFormat128bppRGBAFloat;
	if (wicFormatGUID == GUID_WICPixelFormat128bppRGBFloat) return GUID_WICPixelFormat128bppRGBAFloat;
	if (wicFormatGUID == GUID_WICPixelFormat128bppRGBAFixedPoint) return GUID_WICPixelFormat128bppRGBAFloat;
	if (wicFormatGUID == GUID_WICPixelFormat128bppRGBFixedPoint) return GUID_WICPixelFormat128bppRGBAFloat;
	if (wicFormatGUID == GUID_WICPixelFormat32bppRGBE) return GUID_WICPixelFormat128bppRGBAFloat;
	if (wicFormatGUID == GUID_WICPixelFormat32bppCMYK) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat64bppCMYK) return GUID_WICPixelFormat64bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat40bppCMYKAlpha) return GUID_WICPixelFormat64bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat80bppCMYKAlpha) return GUID_WICPixelFormat64bppRGBA;
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
	if (wicFormatGUID == GUID_WICPixelFormat32bppRGB) return GUID_WICPixelFormat32bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat64bppRGB) return GUID_WICPixelFormat64bppRGBA;
	if (wicFormatGUID == GUID_WICPixelFormat64bppPRGBAHalf) return GUID_WICPixelFormat64bppRGBAHalf;
#endif
	return GUID_WICPixelFormatDontCare;
}

int Texture::GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat)
{
	if (dxgiFormat == DXGI_FORMAT_R32G32B32A32_FLOAT) return 128;
	if (dxgiFormat == DXGI_FORMAT_R16G16B16A16_FLOAT) return 64;
	if (dxgiFormat == DXGI_FORMAT_R16G16B16A16_UNORM) return 64;
	if (dxgiFormat == DXGI_FORMAT_R8G8B8A8_UNORM) return 32;
	if (dxgiFormat == DXGI_FORMAT_B8G8R8A8_UNORM) return 32;
	if (dxgiFormat == DXGI_FORMAT_B8G8R8X8_UNORM) return 32;
	if (dxgiFormat == DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM) return 32;
	if (dxgiFormat == DXGI_FORMAT_R10G10B10A2_UNORM) return 32;
	if (dxgiFormat == DXGI_FORMAT_B5G5R5A1_UNORM) return 16;
	if (dxgiFormat == DXGI_FORMAT_B5G6R5_UNORM) return 16;
	if (dxgiFormat == DXGI_FORMAT_R32_FLOAT) return 32;
	if (dxgiFormat == DXGI_FORMAT_R16_FLOAT) return 16;
	if (dxgiFormat == DXGI_FORMAT_R16_UNORM) return 16;
	if (dxgiFormat == DXGI_FORMAT_R8_UNORM) return 8;
	if (dxgiFormat == DXGI_FORMAT_A8_UNORM) return 8;
	Logger::Error("Bits per pixel not found for format");
	return 0;
}
