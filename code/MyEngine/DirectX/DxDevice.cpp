#include "DxDevice.h"

#include <d3d11.h>
#include <exception>
#include "DxHelper.h"

MyEngine::DirectX::DxDevice::DxDevice(HWND windowHandle)
{
	Init(windowHandle);
}

MyEngine::DirectX::DxDevice::~DxDevice()
{
	Release();
}

void MyEngine::DirectX::DxDevice::Release()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

void MyEngine::DirectX::DxDevice::Render() const
{
}

void MyEngine::DirectX::DxDevice::Init(HWND windowHandle)
{
	DXGI_SWAP_CHAIN_DESC d{};
	d.BufferCount = 2;
	d.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d.BufferDesc.RefreshRate.Numerator = 60;
	d.BufferDesc.RefreshRate.Denominator = 1;
	d.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	d.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d.OutputWindow = windowHandle;
	d.SampleDesc.Count = 1;
	d.SampleDesc.Quality = 0;
	d.Windowed = true;
	d.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	constexpr D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &d, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pContext) != S_OK)
		throw std::exception("CreateDeviceD3D failed");
}
