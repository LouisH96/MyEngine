#include "pch.h"
#include "SamplerState.h"

#include <d3d11.h>

#include "../Gpu.h"

using namespace Rendering;

SamplerState::SamplerState(BorderMode borderX, BorderMode borderY, Filter filter)
{
	Desc desc{ MakeDesc() };
	SetBorderMode(desc, borderX, borderY);
	SetFilter(desc, filter);
	MakeDx(desc);
}

SamplerState::SamplerState(BorderMode borderX, BorderMode borderY, const Float4& outsideValue, Filter filter)
{
	Desc desc{ MakeDesc() };
	SetBorderMode(desc, borderX, borderY);
	SetOutsideValue(desc, outsideValue);
	SetFilter(desc, filter);
	MakeDx(desc);
}

SamplerState::SamplerState(BorderMode borderX, BorderMode borderY, Filter filter, Comparison comparison)
{
	Desc desc{ MakeDesc() };
	SetBorderMode(desc, borderX, borderY);
	SetFilter(desc, filter, comparison);
	MakeDx(desc);
}

SamplerState::SamplerState(BorderMode borderX, BorderMode borderY, const Float4& outsideValue, Filter filter, Comparison comparison)
{
	Desc desc{ MakeDesc() };
	SetBorderMode(desc, borderX, borderY);
	SetOutsideValue(desc, outsideValue);
	SetFilter(desc, filter, comparison);
	MakeDx(desc);
}

SamplerState::~SamplerState()
{
	m_pSamplerState->Release();
}

void SamplerState::ActivateVs(unsigned slot) const
{
	Globals::pGpu->GetContext().VSSetSamplers(slot, 1, &m_pSamplerState);
}

void SamplerState::ActivatePs(unsigned slot) const
{
	Globals::pGpu->GetContext().PSSetSamplers(slot, 1, &m_pSamplerState);
}

void SamplerState::Activate(unsigned slot) const
{
	Globals::pGpu->GetContext().PSSetSamplers(slot, 1, &m_pSamplerState);
	Globals::pGpu->GetContext().VSSetSamplers(slot, 1, &m_pSamplerState);
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

	return {};
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

	return {};
}

constexpr D3D11_FILTER SamplerState::ToDx(Filter filter)
{
	if (filter == Filter::Point)
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	if (filter == Filter::Linear)
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	if (filter == Filter::Anisotropic)
		return D3D11_FILTER_ANISOTROPIC;

	return {};
}

constexpr D3D11_FILTER SamplerState::ToDxWithCompare(Filter filter)
{
	if (filter == Filter::Point)
		return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	if (filter == Filter::Linear)
		return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	if (filter == Filter::Anisotropic)
		return D3D11_FILTER_COMPARISON_ANISOTROPIC;

	return {};
}

constexpr D3D11_COMPARISON_FUNC SamplerState::ToDx(Comparison comparison)
{
	if (comparison == Comparison::Equal)
		return D3D11_COMPARISON_EQUAL;
	if (comparison == Comparison::NotEqual)
		return D3D11_COMPARISON_NOT_EQUAL;
	if (comparison == Comparison::Less)
		return D3D11_COMPARISON_LESS;
	if (comparison == Comparison::LessEqual)
		return D3D11_COMPARISON_LESS_EQUAL;
	if (comparison == Comparison::Greater)
		return D3D11_COMPARISON_GREATER;
	if (comparison == Comparison::GreaterEqual)
		return D3D11_COMPARISON_GREATER_EQUAL;

	return {};
}

constexpr SamplerState::Desc SamplerState::MakeDesc()
{
	D3D11_SAMPLER_DESC desc{};
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	return desc;
}

void SamplerState::SetBorderMode(Desc& desc, BorderMode borderX, BorderMode borderY)
{
	desc.AddressU = ToDx(borderX);
	desc.AddressV = ToDx(borderY);
}

void SamplerState::SetOutsideValue(Desc& desc, const Float4& outsideValue)
{
	desc.BorderColor[0] = outsideValue.x;
	desc.BorderColor[1] = outsideValue.y;
	desc.BorderColor[2] = outsideValue.z;
	desc.BorderColor[3] = outsideValue.w;
}

void SamplerState::SetFilter(Desc& desc, Filter filter)
{
	desc.Filter = ToDx(filter);
}

void SamplerState::SetFilter(Desc& desc, Filter filter, Comparison comparison)
{
	desc.Filter = ToDxWithCompare(filter);
	desc.ComparisonFunc = ToDx(comparison);
}

void SamplerState::MakeDx(const D3D11_SAMPLER_DESC& desc)
{
	const HRESULT result{ Globals::pGpu->GetDevice().CreateSamplerState(&desc, &m_pSamplerState) };
	if (FAILED(result))
		Logger::PrintError("Failed creating samplerState");
}
