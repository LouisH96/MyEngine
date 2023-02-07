#include "pch.h"
#include "Mesh.h"

#include "DxHelper.h"
#include "Shader.h"

MyEngine::App::Wrappers::Gpu::Mesh::Mesh(Gpu& gpu, const Shader::Vertex* pVertices, int nrVertices, const int* pIndices, int nrIndices)
	: m_Gpu(gpu)
	, m_VertexCount(nrVertices)
	, m_VertexStride(sizeof(Shader::Vertex))
	, m_VertexOffset(0)
{
	const D3D11_BUFFER_DESC vertexBufferDesc
	{
		static_cast<unsigned int>(sizeof(Shader::Vertex)) * m_VertexCount,
		D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER,
		0, 0, sizeof(Shader::Vertex)
	};

	const D3D11_SUBRESOURCE_DATA srData{ pVertices,0,0 };
	const HRESULT hr = m_Gpu.GetDevice().CreateBuffer(&vertexBufferDesc, &srData, &m_pVertexBuffer);
	if (FAILED(hr))
		throw std::exception("Mesh::InitVertexBuffer");

	DxHelper::CreateIndexBuffer(gpu.GetDevice(), m_pIndexBuffer, pIndices, nrIndices);

	InitRasterizerState();
}

void MyEngine::App::Wrappers::Gpu::Mesh::Draw() const
{
	m_Gpu.GetContext().RSSetState(m_pRasterizerState);
	m_Gpu.GetContext().DrawIndexed(m_VertexCount, 0, 0);
}

void MyEngine::App::Wrappers::Gpu::Mesh::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = true;

	const HRESULT hr = m_Gpu.GetDevice().CreateRasterizerState(&desc, &m_pRasterizerState);
	if (FAILED(hr))
		throw std::exception("Mesh::InitRasterizerState");
}

void MyEngine::App::Wrappers::Gpu::Mesh::ReleaseRasterizerState()
{
	SAFE_RELEASE(m_pRasterizerState);
}

MyEngine::App::Wrappers::Gpu::Mesh::~Mesh()
{
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	ReleaseRasterizerState();
}

void MyEngine::App::Wrappers::Gpu::Mesh::Activate() const
{
	m_Gpu.GetContext().IASetVertexBuffers(
		0,
		1,
		&m_pVertexBuffer,
		&m_VertexStride,
		&m_VertexOffset);
	m_Gpu.GetContext().IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}
