#include "DxMesh.h"

#include "DxHelper.h"
#include "DxShader.h"

MyEngine::Gpu::Dx::DxMesh::DxMesh(DxGpu& gpu)
	: m_Gpu(gpu)
	, m_VertexCount(6)
	, m_VertexStride(sizeof(DxShader::Vertex))
	, m_VertexOffset(0)
{
	const DxShader::Vertex vertexBuffer[] = {
	   {{0.0f,  0.5f,  0.0f}, {1,0,0}}, // point at top
	   {{0.5f, -0.5f,  0.0f}, {0,1,0}}, // point at bottom-right
	  {{-0.5f, -0.5f,  0.0f}, {0,0,1}}, // point at bottom-left

		{{0.0f, -0.5f, -1.0f}, {1,0,0}},
		{{-0.5f, -0.5f, 0.0f}, {0,1,0}},
		{{0.5f, -0.5f, 0.0f}, {0,0,1}}
	};

	const D3D11_BUFFER_DESC vertexBufferDesc
	{
		static_cast<unsigned int>(sizeof(DxShader::Vertex)) * m_VertexCount,
		D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER,
		0, 0, sizeof(DxShader::Vertex)
	};

	const D3D11_SUBRESOURCE_DATA srData{ &vertexBuffer,0,0 };
	const HRESULT hr = m_Gpu.GetDevice().CreateBuffer(&vertexBufferDesc, &srData, &m_pVertexBuffer);
	if (FAILED(hr))
		throw std::exception("DxMesh::InitVertexBuffer");
}

void MyEngine::Gpu::Dx::DxMesh::Draw() const
{
	m_Gpu.GetContext().Draw(m_VertexCount, 0);
}

MyEngine::Gpu::Dx::DxMesh::~DxMesh()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

void MyEngine::Gpu::Dx::DxMesh::Activate() const
{
	m_Gpu.GetContext().IASetVertexBuffers(
		0,
		1,
		&m_pVertexBuffer,
		&m_VertexStride,
		&m_VertexOffset);
}
