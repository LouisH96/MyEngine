#include "pch.h"
#include "BlendState.h"

#include <d3d11.h>
#include "../Gpu.h"

Rendering::BlendState::BlendState()
	: BlendState{ *Globals::pGpu }
{
}

Rendering::BlendState::BlendState(const Rendering::Gpu& gpu)
{
	D3D11_BLEND_DESC desc{};
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	gpu.GetDevice().CreateBlendState(&desc, &m_pBlendState);
}

Rendering::BlendState::~BlendState()
{
	m_pBlendState->Release();
}

void Rendering::BlendState::Activate(const Rendering::Gpu& gpu) const
{
	gpu.GetContext().OMSetBlendState(m_pBlendState, nullptr, 0xffffffff);
}
