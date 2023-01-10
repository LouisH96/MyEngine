#include "pch.h"
#include "DxGpu.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <exception>

#include "DxCanvas.h"
#include "DxHelper.h"
#include "DxMesh.h"
#include "DxPainter.h"
#include "DxShader.h"
#include "../../App/Window.h"
#include "../../App/Resources.h"

MyEngine::Gpu::Dx::DxGpu::DxGpu(App::Window& window)
	: m_Window{window}
{
	Init();
}

MyEngine::Gpu::Dx::DxGpu::~DxGpu()
{
	Release();
}

void MyEngine::Gpu::Dx::DxGpu::Release()
{
		SAFE_RELEASE(m_pContext)
		SAFE_RELEASE(m_pDevice)
}

MyEngine::Gpu::ICanvas* MyEngine::Gpu::Dx::DxGpu::MakeCanvas()
{
	return new DxCanvas(*this, m_Window);
}

MyEngine::Gpu::IMesh* MyEngine::Gpu::Dx::DxGpu::MakeMesh()
{
	return new DxMesh(*this);
}

MyEngine::Gpu::IShader* MyEngine::Gpu::Dx::DxGpu::MakeShader()
{
	return new DxShader(*this);
}

MyEngine::Gpu::IPainter* MyEngine::Gpu::Dx::DxGpu::MakePainter()
{
	return new DxPainter();
}

void MyEngine::Gpu::Dx::DxGpu::Init()
{
	UINT createDeviceFlags = 0;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	constexpr D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	//if (D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &d, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pContext) != S_OK)
	if (D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &m_pDevice, &featureLevel, &m_pContext) != S_OK)
		throw std::exception("CreateDeviceD3D failed");

}
