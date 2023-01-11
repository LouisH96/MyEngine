#include "pch.h"
#include "Gpu.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <exception>

#include "Canvas.h"
#include "DxHelper.h"
#include "Mesh.h"
#include "Painter.h"
#include "Shader.h"
#include "Wrappers/Win32/Window.h"
#include "App/Resources.h"

MyEngine::Wrappers::Gpu::Gpu::Gpu(Wrappers::Win32::Window& window)
	: m_Window{ window }
{
	Init();
}

MyEngine::Wrappers::Gpu::Gpu::~Gpu()
{
	Release();
}

void MyEngine::Wrappers::Gpu::Gpu::Release()
{
	SAFE_RELEASE(m_pContext)
		SAFE_RELEASE(m_pDevice)
}

MyEngine::Wrappers::Gpu::Canvas* MyEngine::Wrappers::Gpu::Gpu::MakeCanvas()
{
	return new Canvas(*this, m_Window);
}

MyEngine::Wrappers::Gpu::Mesh* MyEngine::Wrappers::Gpu::Gpu::MakeMesh()
{
	return new Mesh(*this);
}

MyEngine::Wrappers::Gpu::Shader* MyEngine::Wrappers::Gpu::Gpu::MakeShader()
{
	return new Shader(*this);
}

MyEngine::Wrappers::Gpu::Painter* MyEngine::Wrappers::Gpu::Gpu::MakePainter()
{
	return new Painter();
}

void MyEngine::Wrappers::Gpu::Gpu::Init()
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
