#include "pch.h"
#include "SamplerState.h"

#include <d3d11.h>

#include "../Gpu.h"

Rendering::SamplerState::SamplerState()
{
	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	const HRESULT result{ Globals::pGpu->GetDevice().CreateSamplerState(&desc, &m_pSamplerState) };
	if (FAILED(result))
		Logger::PrintError("Failed creating samplerState");
}

Rendering::SamplerState::~SamplerState()
{
	m_pSamplerState->Release();
}

void Rendering::SamplerState::ActivateVs() const
{
	Globals::pGpu->GetContext().VSSetSamplers(0, 1, &m_pSamplerState);
}

void Rendering::SamplerState::ActivatePs() const
{
	Globals::pGpu->GetContext().PSSetSamplers(0, 1, &m_pSamplerState);
}

void Rendering::SamplerState::Activate() const
{
	Globals::pGpu->GetContext().PSSetSamplers(0, 1, &m_pSamplerState);
	Globals::pGpu->GetContext().VSSetSamplers(0, 1, &m_pSamplerState);
}
