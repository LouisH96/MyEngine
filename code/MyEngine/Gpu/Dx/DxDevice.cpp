#include "DxDevice.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <exception>

#include "DxCanvas.h"
#include "DxHelper.h"
#include "DxMesh.h"
#include "DxShader.h"
#include "../../App/IWindow.h"
#include "../../App/Resources.h"
#include "../../App/Win32/Win32Window.h"

MyEngine::Gpu::Dx::DxDevice::DxDevice(HWND windowHandle)
{
	Init(windowHandle);
}

MyEngine::Gpu::Dx::DxDevice::~DxDevice()
{
	Release();
}

void MyEngine::Gpu::Dx::DxDevice::Release()
{
		SAFE_RELEASE(m_pContext)
		SAFE_RELEASE(m_pDevice)
}

MyEngine::Gpu::ICanvas* MyEngine::Gpu::Dx::DxDevice::MakeCanvas(App::IWindow& window)
{
	App::Win32::Win32Window* pWin32 = dynamic_cast<App::Win32::Win32Window*>(&window);
	if (!pWin32)
		throw std::exception("DxDevice::MakeCanvas - window should be Win32App");

	return new DxCanvas(*this, *pWin32);
}

MyEngine::Gpu::IMesh* MyEngine::Gpu::Dx::DxDevice::MakeMesh()
{
	return new DxMesh(*this);
}

MyEngine::Gpu::IShader* MyEngine::Gpu::Dx::DxDevice::MakeShader()
{
	return new DxShader(*this);
}

void MyEngine::Gpu::Dx::DxDevice::Init(HWND windowHandle)
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