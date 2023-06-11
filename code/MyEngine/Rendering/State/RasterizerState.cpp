#include "pch.h"
#include "RasterizerState.h"

#include "../Gpu.h"

Rendering::RasterizerState::RasterizerState(bool isWireframe)
	: RasterizerState{ *Globals::pGpu, isWireframe }
{
}

Rendering::RasterizerState::RasterizerState(const Gpu& gpu, bool isWireframe)
{
	D3D11_RASTERIZER_DESC desc{};
	if (isWireframe)
	{
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;
		desc.DepthBias = -50;
	}
	else
	{
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
	}
	desc.DepthClipEnable = true;

	const HRESULT result = gpu.GetDevice().CreateRasterizerState(&desc, &m_pState);
	if (FAILED(result))
		Logger::PrintError("RasterizerState");
}

Rendering::RasterizerState::~RasterizerState()
{
	m_pState->Release();
}

void Rendering::RasterizerState::Activate(const Gpu& gpu) const
{
	gpu.GetContext().RSSetState(m_pState);
}
