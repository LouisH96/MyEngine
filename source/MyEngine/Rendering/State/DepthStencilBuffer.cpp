#include "pch.h"
#include "DepthStencilBuffer.h"

#include "..\RenderOptions.h"

using namespace Rendering;
using namespace Dx;

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
	desc.ViewDimension = RenderOptions::UsingMultiSampling()
		? D3D11_DSV_DIMENSION_TEXTURE2DMS
		: D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;
	const HRESULT result{
		Globals::pGpu->GetDevice().CreateDepthStencilView(
			pTexture, &desc, &m_pView)
	};
	if (DxHelper::OnFail("[DepthStencilBuffer::CreateView]", result))
		success = false;

	pTexture->Release();
	return success;
}

bool DepthStencilBuffer::Update(const Int2& size, bool asShaderResource)
{
	SAFE_RELEASE(m_pView);
	return Init(size, asShaderResource);
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
	desc.SampleDesc.Count = RenderOptions::Samples;
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
	if (DxHelper::OnFail("[DepthStencilBuffer::MakeTexture] Failed creating texture.", result)) {
		return nullptr;
	}
	return pTexture;
}

ID3D11ShaderResourceView* DepthStencilBuffer::MakeShaderResourceView() const
{
	//Get
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
	m_pView->GetDesc(&dsViewDesc);

	ID3D11Resource* dsResource;
	m_pView->GetResource(&dsResource);

	//Make ShaderResourceViewDesc
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
	resourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	resourceViewDesc.ViewDimension = RenderOptions::UsingMultiSampling()
		? D3D11_SRV_DIMENSION_TEXTURE2DMS
		: D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = 1;

	//Make ShaderResourceView
	ID3D11ShaderResourceView* pShaderResourceView;
	const HRESULT result{
		Globals::pGpu->GetDevice().CreateShaderResourceView(dsResource, &resourceViewDesc, &pShaderResourceView)
	};
	SAFE_RELEASE(dsResource);

	if (DxHelper::OnFail("[DepthStencilBuffer::MakeShaderResourceView]", result))
		SAFE_RELEASE(pShaderResourceView);

	return pShaderResourceView;
}