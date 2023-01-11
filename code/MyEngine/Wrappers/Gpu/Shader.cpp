#include "pch.h"
#include "Shader.h"

#include <d3dcompiler.h>

#include "Gpu.h"
#include "DxHelper.h"
#include "../../App/Resources.h"
#include "../../Game/Camera/Camera.h"

MyEngine::Wrappers::Gpu::Shader::Shader(Gpu& gpu)
	: m_Gpu(gpu)
{
	InitShaders();
	InitInputLayout();
	InitCBuffer();
}

void MyEngine::Wrappers::Gpu::Shader::Activate() const
{
	m_Gpu.GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Gpu.GetContext().IASetInputLayout(m_pInputLayout);
	m_Gpu.GetContext().VSSetShader(m_pVertexShader, nullptr, 0);
	m_Gpu.GetContext().PSSetShader(m_pPixelShader, nullptr, 0);
	m_Gpu.GetContext().VSSetConstantBuffers(0, 1, &m_pCBuffer);
}

void MyEngine::Wrappers::Gpu::Shader::OnCamUpdated(Game::Camera::Camera& camera)
{
	UpdateCBuffer(*reinterpret_cast<Game::Camera::Camera*>(&camera));
}

MyEngine::Wrappers::Gpu::Shader::~Shader()
{
	SAFE_RELEASE(m_pInputLayout)
		SAFE_RELEASE(m_pPixelShader)
		SAFE_RELEASE(m_pVertexShader)
		ReleaseCBuffer();
}

void MyEngine::Wrappers::Gpu::Shader::InitShaders()
{
	const std::wstring path = App::Resources::GetShaderPath(L"shader.hlsl");
	DxHelper::CreateVertexShader(m_Gpu.GetDevice(), path, "vs_main", m_pVertexShader);
	DxHelper::CreatePixelShader(m_Gpu.GetDevice(), path, "ps_main", m_pPixelShader);
}

void MyEngine::Wrappers::Gpu::Shader::InitInputLayout()
{
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

	result = m_Gpu.GetDevice().CreateInputLayout(
		inputElementDesc, ARRAYSIZE(inputElementDesc),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&m_pInputLayout);

	SAFE_RELEASE(pBlob);
	SAFE_RELEASE(pErrorBlob);

	if (FAILED(result))
		throw std::exception("DxDevice::CreateInputLayout");
}

void MyEngine::Wrappers::Gpu::Shader::InitCBuffer()
{
	DxHelper::CreateDynamicConstantBuffer<CBuffer>(m_Gpu.GetDevice(), m_pCBuffer);
}

void MyEngine::Wrappers::Gpu::Shader::ReleaseCBuffer()
{
	SAFE_RELEASE(m_pCBuffer)
}

void MyEngine::Wrappers::Gpu::Shader::UpdateCBuffer(const Game::Camera::Camera& camera) const
{
	DxHelper::UpdateBuffer(m_Gpu.GetContext(), *m_pCBuffer, camera.GetViewProjMatrix());
}
