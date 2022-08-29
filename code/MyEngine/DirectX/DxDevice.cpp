#include "DxDevice.h"

#include <d3d11.h>
#include <d3dcompiler.h>
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
	SAFE_RELEASE(m_pSwapChain)
		SAFE_RELEASE(m_pContext)
		SAFE_RELEASE(m_pDevice)
}

void MyEngine::DirectX::DxDevice::Render() const
{
	TempRender();
}

void MyEngine::DirectX::DxDevice::Init(HWND windowHandle)
{
	DXGI_SWAP_CHAIN_DESC1 d{};
	d.BufferCount = 2;
	d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	d.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d.SampleDesc.Count = 1;
	d.SampleDesc.Quality = 0;
	d.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	d.Stereo = true;

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

	IDXGIDevice2* pDXGIDevice;
	HRESULT hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice2), (void**)&pDXGIDevice);

	IDXGIAdapter* pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);

	IDXGIFactory2* pIDXGIFactory;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pIDXGIFactory);

	if (pIDXGIFactory->CreateSwapChainForHwnd(m_pDevice, windowHandle, &d, nullptr, nullptr, &m_pSwapChain) != S_OK)
		throw std::exception("CreateSwapChain1 Failed");

	TempInit(windowHandle);
}

void MyEngine::DirectX::DxDevice::TempInit(HWND windowHandle)
{
	//----| Shaders |----
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);

	std::wstring path = buffer;
	path = path.substr(0, path.find_last_of('\\'));
	path = path.append(LR"(\resources)");
	path = path.append(LR"(\Shaders\shader.hlsl)");
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
			float3 pos : POS;
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
		{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	float vertex_data_array[] = {
	   0.0f,  0.5f,  0.0f, // point at top
	   0.5f, -0.5f,  0.0f, // point at bottom-right
	  -0.5f, -0.5f,  0.0f, // point at bottom-left
	};
	const D3D11_BUFFER_DESC vertexBufferDesc
	{
		sizeof(float) * 3 * 3,
		D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER,
		0, 0, sizeof(float) * 3
	};
	const D3D11_SUBRESOURCE_DATA srData{ &vertex_data_array,0,0 };
	HRESULT hr = m_pDevice->CreateBuffer(&vertexBufferDesc, &srData, &m_pVertexBuffer);
	if (FAILED(hr))
		throw std::exception("DxDevice::CreateVertexBuffer");

	//----| ViewPort |----
	RECT winRect;
	GetClientRect(windowHandle, &winRect);
	D3D11_VIEWPORT viewport = {
	  0.0f,
	  0.0f,
	  (FLOAT)(winRect.right - winRect.left),
	  (FLOAT)(winRect.bottom - winRect.top),
	  0.0f,
	  1.0f };
	m_pContext->RSSetViewports(1, &viewport);

	//----| RenderTarget |----
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
		m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pMainRenderTargetView);
	pBackBuffer->Release();
}

void MyEngine::DirectX::DxDevice::TempRender() const
{
	/* clear the back buffer to cornflower blue for the new frame */
	float background_colour[4] = {
	  0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
	m_pContext->ClearRenderTargetView(
		m_pMainRenderTargetView, background_colour);

	UINT vertex_stride = 3 * sizeof(float);
	UINT vertex_offset = 0;
	UINT vertex_count = 3;
	m_pContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, NULL);
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

	DXGI_PRESENT_PARAMETERS param{ 0,nullptr,0,nullptr };
	m_pSwapChain->Present1(0, 0, &param);
}
