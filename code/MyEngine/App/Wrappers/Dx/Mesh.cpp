#include "pch.h"
#include "Mesh.h"

using namespace App::Wrappers::Dx;

App::Wrappers::Dx::Mesh::Mesh(const Gpu& gpu, unsigned vertexStride, const void* pVertices, int nrVertices, const int* pIndices,
	int nrIndices, Topology topology)
	: m_Gpu(gpu)
	, m_VertexCount(nrVertices)
	, m_VertexStride(vertexStride)
	, m_VertexOffset(0)
	, m_IndexCount(nrIndices)
	, m_Topology(ToDxTopology(topology))
{
	const D3D11_BUFFER_DESC vertexBufferDesc
	{
		static_cast<unsigned int>(vertexStride) * m_VertexCount,
		D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER,
		0, 0, vertexStride
	};

	const D3D11_SUBRESOURCE_DATA srData{ pVertices,0,0 };
	const HRESULT hr = m_Gpu.GetDevice().CreateBuffer(&vertexBufferDesc, &srData, &m_pVertexBuffer);
	if (FAILED(hr))
		throw std::exception("Mesh::InitVertexBuffer");

	if (nrIndices > 0)
		DxHelper::CreateIndexBuffer(gpu.GetDevice(), m_pIndexBuffer, pIndices, nrIndices);
}

Mesh::~Mesh()
{
	if (m_IndexCount > 0)
		SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}

void Mesh::Activate() const
{
	m_Gpu.GetContext().IASetVertexBuffers(
		0,
		1,
		&m_pVertexBuffer,
		&m_VertexStride,
		&m_VertexOffset);
	m_Gpu.GetContext().IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Gpu.GetContext().IASetPrimitiveTopology(m_Topology);
}

void App::Wrappers::Dx::Mesh::ActivateUnindexed() const
{
	m_Gpu.GetContext().IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer,
		&m_VertexStride,
		&m_VertexOffset);
	m_Gpu.GetContext().IASetPrimitiveTopology(m_Topology);
}

void Mesh::Draw() const
{
	m_Gpu.GetContext().DrawIndexed(m_IndexCount, 0, 0);
}

void Mesh::DrawNotIndexed() const
{
	m_Gpu.GetContext().Draw(m_VertexCount, m_VertexOffset);
}

D3D11_PRIMITIVE_TOPOLOGY Mesh::ToDxTopology(Topology topology)
{
	switch (topology)
	{
	case Topology::TriangleList: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case Topology::LineStrip: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case Topology::Unknown:
	default: return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}
}
