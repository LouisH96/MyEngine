#include "pch.h"
#include "SamplerState.h"

#include <d3d11.h>

#include "../Gpu.h"

using namespace Rendering;

SamplerState::SamplerState(BorderMode borderX, BorderMode borderY)
{
	const D3D11_SAMPLER_DESC desc{ MakeDesc(borderX, borderY) };
	MakeDx(desc);
}

SamplerState::SamplerState(BorderMode borderX, BorderMode borderY, Float4 value)
{
	D3D11_SAMPLER_DESC desc{ MakeDesc(borderX, borderY) };
	desc.BorderColor[0] = value.x;
	desc.BorderColor[1] = value.y;
	desc.BorderColor[2] = value.z;
	desc.BorderColor[3] = value.w;
	MakeDx(desc);
}

SamplerState::~SamplerState()
{
	m_pSamplerState->Release();
}

void SamplerState::ActivateVs() const
{
	Globals::pGpu->GetContext().VSSetSamplers(0, 1, &m_pSamplerState);
}

void SamplerState::ActivatePs() const
{
	Globals::pGpu->GetContext().PSSetSamplers(0, 1, &m_pSamplerState);
}

void SamplerState::Activate() const
{
	Globals::pGpu->GetContext().PSSetSamplers(0, 1, &m_pSamplerState);
	Globals::pGpu->GetContext().VSSetSamplers(0, 1, &m_pSamplerState);
}

constexpr D3D11_TEXTURE_ADDRESS_MODE SamplerState::ToDx(BorderMode mode)
{
	if (mode == BorderMode::Repeat)
		return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	if (mode == BorderMode::Mirror)
		return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	if (mode == BorderMode::Clamp)
		return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	if (mode == BorderMode::Value)
		return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	if (mode == BorderMode::MirrorOnce)
		return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
}

constexpr SamplerState::BorderMode SamplerState::ToBorderMode(D3D11_TEXTURE_ADDRESS_MODE mode)
{
	if (mode == D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP)
		return BorderMode::Repeat;
	if (mode == D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR)
		return BorderMode::Mirror;
	if (mode == D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP)
		return BorderMode::Clamp;
	if (mode == D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER)
		return BorderMode::Value;
	if (mode == D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR_ONCE)
		return BorderMode::MirrorOnce;
}

constexpr D3D11_SAMPLER_DESC SamplerState::MakeDesc(BorderMode borderX, BorderMode borderY)
{
	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = ToDx(borderX);
	desc.AddressV = ToDx(borderY);
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	return desc;
}

void SamplerState::MakeDx(const D3D11_SAMPLER_DESC& desc)
{
	const HRESULT result{ Globals::pGpu->GetDevice().CreateSamplerState(&desc, &m_pSamplerState) };
	if (FAILED(result))
		Logger::PrintError("Failed creating samplerState");
}
