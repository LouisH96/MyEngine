#include "pch.h"
#include "DepthStencilBuffer.h"

using namespace Rendering;

bool DepthStencilBuffer::Init(const Int2& size, bool asShaderResource)
{
	bool success{ true };

	//Texture
	ID3D11Texture2D* pTexture{ MakeTexture(size, asShaderResource) };
	if (!pTexture)
		return false;

	//View
	D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;
	const HRESULT result{
		Globals::pGpu->GetDevice().CreateDepthStencilView(
			pTexture, &desc, &m_pView)
	};
	if (FAILED(result)) {
		Logger::PrintError("[DepthStencilBuffer] Failed creating view.");
		success = false;
	}
	pTexture->Release();
	return success;
}

bool DepthStencilBuffer::Update(const Int2& size)
{
	SAFE_RELEASE(m_pView);
	return Init(size);
}

void DepthStencilBuffer::Clear()
{
	Globals::pGpu->GetContext().ClearDepthStencilView(m_pView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

DepthStencilBuffer::~DepthStencilBuffer()
{
	SAFE_RELEASE(m_pView);
}

ID3D11Texture2D* DepthStencilBuffer::MakeTexture(const Int2& size, bool asShaderResource)
{
	ID3D11Texture2D* pTexture{ nullptr };
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = size.x;
	desc.Height = size.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = asShaderResource ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	if (asShaderResource)
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	const HRESULT result{
		Globals::pGpu->GetDevice().CreateTexture2D(&desc, nullptr, &pTexture)
	};
	if (FAILED(result)) {
		Logger::PrintError("[DepthStencilBuffer::MakeTexture] Failed creating texture.");
		return nullptr;
	}
	return pTexture;
}
