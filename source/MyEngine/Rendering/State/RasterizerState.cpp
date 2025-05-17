#include "pch.h"
#include "RasterizerState.h"

#include "../Gpu.h"

using namespace Rendering;

RasterizerState::RasterizerState(Culling culling, bool isWireframe)
{
	D3D11_RASTERIZER_DESC desc{};
	if (isWireframe)
	{
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.DepthBias = -50;
	}
	else
	{
		desc.FillMode = D3D11_FILL_SOLID;
	}
	desc.CullMode = ToDx(culling);
	desc.DepthClipEnable = true;

	const HRESULT result = Globals::pGpu->GetDevice().CreateRasterizerState(&desc, &m_pState);
	if (FAILED(result))
		Logger::Error("RasterizerState");
}

RasterizerState::~RasterizerState()
{
	m_pState->Release();
}

void RasterizerState::Activate() const
{
	Globals::pGpu->GetContext().RSSetState(m_pState);
}

RasterizerState RasterizerState::MakeDefault(bool isWireframe)
{
	if (isWireframe)
		return MakeWireframe();
	else
		return RasterizerState{};
}

RasterizerState RasterizerState::MakeWireframe()
{
	return RasterizerState{ Culling::None, true };
}

D3D11_CULL_MODE RasterizerState::ToDx(Culling culling)
{
	switch (culling)
	{
	case Culling::Front:
		return D3D11_CULL_FRONT;

	case Culling::Back:
		return D3D11_CULL_BACK;

	default:
	case Culling::None:
		return D3D11_CULL_NONE;
	}
}
