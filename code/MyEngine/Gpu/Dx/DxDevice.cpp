#include "DxDevice.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <exception>

#include "DxCanvas.h"
#include "DxHelper.h"
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
		SAFE_RELEASE(m_pVertexBuffer)
		SAFE_RELEASE(m_pInputLayout)
		SAFE_RELEASE(m_pVertexShader)
		SAFE_RELEASE(m_pPixelShader)
		SAFE_RELEASE(m_pContext)
		SAFE_RELEASE(m_pDevice)
}

MyEngine::Gpu::ICanvas* MyEngine::Gpu::Dx::DxDevice::MakeCanvas(App::IWindow& window)
{
	App::Win32::Win32Window* pWin32 = dynamic_cast<App::Win32::Win32Window*>(&window);
	if (!pWin32)
		throw std::exception("DxDevice::MakeCanvas - window should be Win32App");

	return new DxCanvas(*m_pDevice, *m_pContext, *pWin32);
}

void MyEngine::Gpu::Dx::DxDevice::Temp() const
{
	TempRender();
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

	TempInit();
}

void MyEngine::Gpu::Dx::DxDevice::TempInit()
{
	//----| Shaders |----
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);

	const std::wstring path = App::Resources::GetShaderPath(L"shader.hlsl");
	DxHelper::CreateVertexShader(*m_pDevice, path, "vs_main", m_pVertexShader);
	DxHelper::CreatePixelShader(*m_pDevice, path, "ps_main", m_pPixelShader);

	//----| InputLayout |----
	//--- DummyShader ---
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags
		|= D3DCOMPILE_DEBUG; // add more debug output
#endif
	const std::string dummyShader
	{
		R"V0G0N(
		struct Vertex{
			float3 pos : POSITION;
			float3 col : COLOR;
		};

		struct Pixel { float4 pos : VS_POSITION; };
		Pixel vs_main(Vertex vertex) { return (Pixel)0; }
		)V0G0N"
	};
	const std::string entryPoint = "vs_main";
	const std::string target = "vs_5_0";

	ID3DBlob* pBlob{};
	ID3DBlob* pErrorBlob{};

	HRESULT result = D3DCompile(dummyShader.c_str(), dummyShader.size(),
		nullptr, nullptr, nullptr,
		entryPoint.c_str(), target.c_str(), flags,
		0, &pBlob, &pErrorBlob);

	if (FAILED(result)) {
		std::string error = "DxDevice::CreateInputLayout";
		if (pErrorBlob)
		{
			error = static_cast<char*>(pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		if (pBlob)
			pBlob->Release();
		throw std::exception{ error.c_str() };
	}

	//--- InputLayout ---
	constexpr D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	result = m_pDevice->CreateInputLayout(
		inputElementDesc, ARRAYSIZE(inputElementDesc),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&m_pInputLayout);

	SAFE_RELEASE(pBlob);
	SAFE_RELEASE(pErrorBlob);

	if (FAILED(result))
		throw std::exception("DxDevice::CreateInputLayout");

	//----| VertexBuffer |----
	const Vertex vertexBuffer[] = {
	   {{0.0f,  0.5f,  0.0f}, {1,0,0}}, // point at top
	   {{0.5f, -0.5f,  0.0f}, {0,1,0}}, // point at bottom-right
	  {{-0.5f, -0.5f,  0.0f}, {0,0,1}}, // point at bottom-left
	};
	const D3D11_BUFFER_DESC vertexBufferDesc
	{
		sizeof(Vertex) * 3,
		D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER,
		0, 0, sizeof(Vertex)
	};
	const D3D11_SUBRESOURCE_DATA srData{ &vertexBuffer,0,0 };
	HRESULT hr = m_pDevice->CreateBuffer(&vertexBufferDesc, &srData, &m_pVertexBuffer);
	if (FAILED(hr))
		throw std::exception("DxDevice::CreateVertexBuffer");
}

void MyEngine::Gpu::Dx::DxDevice::TempRender() const
{
	UINT vertex_stride = sizeof(Vertex);
	UINT vertex_offset = 0;
	UINT vertex_count = 3;
	m_pContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pContext->IASetInputLayout(m_pInputLayout);
	m_pContext->IASetVertexBuffers(
		0,
		1,
		&m_pVertexBuffer,
		&vertex_stride,
		&vertex_offset);
	m_pContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pContext->Draw(vertex_count, 0);

}