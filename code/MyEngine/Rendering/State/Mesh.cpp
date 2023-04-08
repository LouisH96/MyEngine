#include "pch.h"
#include "Mesh.h"

using namespace Rendering;

Mesh::Mesh(const Gpu& gpu, unsigned vertexStride, const void* pVertices, int nrVertices, const int* pIndices,
	int nrIndices, Topology topology, bool immutable)
	: m_VertexCount(nrVertices)
	, m_VertexStride(vertexStride)
	, m_IndexCount(nrIndices)
	, m_Topology(ToDxTopology(topology))
	, m_fpActivate(&Mesh::ActivateIndexed)
	, m_fpDraw(&Mesh::DrawIndexed)
{
	InitVertexBuffer(gpu, pVertices, immutable);
	Dx::DxHelper::CreateIndexBuffer(gpu.GetDevice(), m_pIndexBuffer, pIndices, nrIndices);
}

Mesh::Mesh(const Gpu& gpu, unsigned vertexStride, const void* pVertices, int nrVertices, Topology topology, bool immutable)
	: m_VertexCount(nrVertices)
	, m_VertexStride(vertexStride)
	, m_IndexCount(0)
	, m_Topology(ToDxTopology(topology))
	, m_fpActivate(&Mesh::ActivateUnindexed)
	, m_fpDraw(&Mesh::DrawUnindexed)
{
	InitVertexBuffer(gpu, pVertices, immutable);
}

void Mesh::InitVertexBuffer(const Gpu& gpu, const void* pInitVertices, bool immutable)
{
	const D3D11_BUFFER_DESC vertexBufferDesc
	{
		m_VertexStride * m_VertexCount,
		immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC,
		D3D11_BIND_VERTEX_BUFFER,
		immutable ? 0 : D3D11_CPU_ACCESS_WRITE,
		0, m_VertexStride
	};

	const D3D11_SUBRESOURCE_DATA srData{ pInitVertices,0,0 };
	const HRESULT hr = gpu.GetDevice().CreateBuffer(&vertexBufferDesc, &srData, &m_pVertexBuffer);
	if (FAILED(hr))
		throw std::exception("Mesh::InitVertexBuffer");
}

Mesh::~Mesh()
{
	if (m_IndexCount > 0)
		SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}

void Mesh::Activate(const Gpu& gpu) const
{
	(this->*m_fpActivate)(gpu);
}

void Mesh::Draw(const Gpu& gpu) const
{
	(this->*m_fpDraw)(gpu);
}

void Mesh::ActivateIndexed(const Gpu& gpu) const
{
	gpu.GetContext().IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer,
		&m_VertexStride,
		&m_VertexOffset);
	gpu.GetContext().IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gpu.GetContext().IASetPrimitiveTopology(m_Topology);
}

void Mesh::ActivateUnindexed(const Gpu& gpu) const
{
	gpu.GetContext().IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer,
		&m_VertexStride,
		&m_VertexOffset);
	gpu.GetContext().IASetPrimitiveTopology(m_Topology);
}

void Mesh::DrawIndexed(const Gpu& gpu) const
{
	gpu.GetContext().DrawIndexed(m_IndexCount, 0, 0);
}

void Mesh::DrawUnindexed(const Gpu& gpu) const
{
	gpu.GetContext().Draw(m_VertexCount, m_VertexOffset);
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
