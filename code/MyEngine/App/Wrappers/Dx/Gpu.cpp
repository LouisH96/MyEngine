#include "pch.h"
#include "Gpu.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <exception>

#include "Canvas.h"
#include "DxHelper.h"
#include "App/Wrappers/Win32/Window.h"

MyEngine::App::Wrappers::Dx::Gpu::Gpu(App::Wrappers::Win32::Window& window)
	: m_Window{ window }
{
	Init();
}

MyEngine::App::Wrappers::Dx::Gpu::~Gpu()
{
	Release();
}

void MyEngine::App::Wrappers::Dx::Gpu::Release()
{
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

MyEngine::App::Wrappers::Dx::Canvas* MyEngine::App::Wrappers::Dx::Gpu::MakeCanvas()
{
	return new Canvas(*this, m_Window);
}

void MyEngine::App::Wrappers::Dx::Gpu::Init()
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
