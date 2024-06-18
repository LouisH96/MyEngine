#include "pch.h"
#include "DepthStencilState.h"

#include <d3d11.h>
#include <Rendering/Gpu.h>

Rendering::DepthStencilState::DepthStencilState(bool useDepth)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc{};

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = useDepth ? D3D11_COMPARISON_LESS_EQUAL : D3D11_COMPARISON_ALWAYS;
	dsDesc.DepthWriteMask = useDepth ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	const HRESULT result{ Globals::pGpu->GetDevice().CreateDepthStencilState(&dsDesc, &m_pState) };
	if (FAILED(result)) Logger::PrintError("Failed creating DepthStencilBuffer");
}

Rendering::DepthStencilState::~DepthStencilState()
{
	if (m_pState) m_pState->Release();
	m_pState = nullptr;
}

Rendering::DepthStencilState::DepthStencilState(DepthStencilState&& other) noexcept
	: m_pState{ other.m_pState }
{
	other.m_pState = nullptr;
}

Rendering::DepthStencilState& Rendering::DepthStencilState::operator=(DepthStencilState&& other) noexcept
{
	if (&other == this) return *this;
	m_pState = other.m_pState;
	other.m_pState = nullptr;
	return *this;
}

void Rendering::DepthStencilState::Activate() const
{
	Globals::pGpu->GetContext().OMSetDepthStencilState(m_pState, 1);
}
