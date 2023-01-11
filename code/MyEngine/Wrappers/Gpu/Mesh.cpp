#include "pch.h"
#include "Mesh.h"

#include "DxHelper.h"
#include "Shader.h"

MyEngine::Wrappers::Gpu::Mesh::Mesh(Gpu& gpu)
	: m_Gpu(gpu)
	, m_VertexCount(6)
	, m_VertexStride(sizeof(Shader::Vertex))
	, m_VertexOffset(0)
{
	const Shader::Vertex vertexBuffer[] = {
	   {{0.0f,  0.5f,  0.0f}, {1,0,0}}, // point at top
	   {{0.5f, -0.5f,  0.0f}, {0,1,0}}, // point at bottom-right
	  {{-0.5f, -0.5f,  0.0f}, {0,0,1}}, // point at bottom-left

		{{0.0f, -0.5f, -1.0f}, {1,0,0}},
		{{-0.5f, -0.5f, 0.0f}, {0,1,0}},
		{{0.5f, -0.5f, 0.0f}, {0,0,1}}
	};

	const D3D11_BUFFER_DESC vertexBufferDesc
	{
		static_cast<unsigned int>(sizeof(Shader::Vertex)) * m_VertexCount,
		D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER,
		0, 0, sizeof(Shader::Vertex)
	};

	const D3D11_SUBRESOURCE_DATA srData{ &vertexBuffer,0,0 };
	const HRESULT hr = m_Gpu.GetDevice().CreateBuffer(&vertexBufferDesc, &srData, &m_pVertexBuffer);
	if (FAILED(hr))
		throw std::exception("Mesh::InitVertexBuffer");

	InitRasterizerState();
}

void MyEngine::Wrappers::Gpu::Mesh::Draw() const
{
	m_Gpu.GetContext().RSSetState(m_pRasterizerState);
	m_Gpu.GetContext().Draw(m_VertexCount, 0);
}

void MyEngine::Wrappers::Gpu::Mesh::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = true;

	const HRESULT hr = m_Gpu.GetDevice().CreateRasterizerState(&desc, &m_pRasterizerState);
	if (FAILED(hr))
		throw std::exception("Mesh::InitRasterizerState");
}

void MyEngine::Wrappers::Gpu::Mesh::ReleaseRasterizerState()
{
	SAFE_RELEASE(m_pRasterizerState);
}

MyEngine::Wrappers::Gpu::Mesh::~Mesh()
{
	SAFE_RELEASE(m_pVertexBuffer);
	ReleaseRasterizerState();
}

void MyEngine::Wrappers::Gpu::Mesh::Activate() const
{
	m_Gpu.GetContext().IASetVertexBuffers(
		0,
		1,
		&m_pVertexBuffer,
		&m_VertexStride,
		&m_VertexOffset);
}
