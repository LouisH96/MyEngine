#include "pch.h"
#include "Gpu.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Canvas.h"
#include "Dx/DxHelper.h"
#include "App/Win32/Window.h"

using namespace Rendering;

Gpu::Gpu()
{
	Init();

	if (Globals::pGpu) Logger::PrintError("A second gpu is made");
	Globals::pGpu = this;
}

Gpu::~Gpu()
{
	Release();
}

void Gpu::Release()
{
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

void Gpu::Init()
{
	//Select best gpu
	IDXGIFactory* pFactory{};

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory))))
		Logger::PrintError("[Gpu::Init] failed creating factory");

	IDXGIAdapter* pAdapter{};
	IDXGIAdapter* pBestAdapter{};
	SIZE_T highestVram{ 0 };
	for (unsigned i{ 0 }; !FAILED(pFactory->EnumAdapters(i, &pAdapter)); ++i)
	{
		DXGI_ADAPTER_DESC desc{};
		pAdapter->GetDesc(&desc);
		if (desc.DedicatedVideoMemory > highestVram)
		{
			SAFE_RELEASE(pBestAdapter);
			pBestAdapter = pAdapter;
			highestVram = desc.DedicatedVideoMemory;
		}
		else
			pAdapter->Release();
	}
	SAFE_RELEASE(pFactory);
	
	if (!pBestAdapter)
		Logger::PrintError("[Gpu::Init] no adapter found");

	UINT createDeviceFlags = 0;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	constexpr D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	//if (D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &d, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pContext) != S_OK)
	if (D3D11CreateDevice(pBestAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &m_pDevice, &featureLevel, &m_pContext) != S_OK)
		Logger::PrintError("[Gpu::Init] CreateDeviceD3D failed");

	pBestAdapter->Release();
}
