#include "pch.h"
#include "RasterizerState.h"

#include "App/Wrappers/Dx/Gpu.h"

Dx::RasterizerState::RasterizerState(const App::Wrappers::Dx::Gpu& gpu, bool isWireframe)
{
	D3D11_RASTERIZER_DESC desc{};
	if(isWireframe)
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

Dx::RasterizerState::~RasterizerState()
{
	m_pState->Release();
}

void Dx::RasterizerState::Activate(const MyEngine::App::Wrappers::Dx::Gpu& gpu) const
{
	gpu.GetContext().RSSetState(m_pState);
}
