#include "pch.h"
#include "Mesh.h"

using namespace Rendering;

Mesh::Mesh(unsigned vertexStride, const void* pVertices, int nrVertices, const int* pIndices,
	int nrIndices, Topology topology, bool immutable)
	: m_VertexCount(nrVertices)
	, m_VertexStride(vertexStride)
	, m_IndexCount(nrIndices)
	, m_Topology(ToDxTopology(topology))
	, m_fpActivate(&Mesh::ActivateIndexed)
	, m_fpDraw(&Mesh::DrawIndexed)
{
	InitVertexBuffer(pVertices, immutable);
	Dx::DxHelper::CreateIndexBuffer(m_pIndexBuffer, pIndices, nrIndices);
}

Mesh::Mesh(unsigned vertexStride, const void* pVertices, int nrVertices, Topology topology, bool immutable)
	: m_VertexCount(nrVertices)
	, m_VertexStride(vertexStride)
	, m_IndexCount(0)
	, m_Topology(ToDxTopology(topology))
	, m_fpActivate(&Mesh::ActivateUnindexed)
	, m_fpDraw(&Mesh::DrawUnindexed)
{
	InitVertexBuffer(pVertices, immutable);
}

void Mesh::InitVertexBuffer(const void* pInitVertices, bool immutable)
{
	const D3D11_BUFFER_DESC vertexBufferDesc
	{
		m_VertexStride * m_VertexCount,
		immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC,
		D3D11_BIND_VERTEX_BUFFER,
		immutable ? 0u : D3D11_CPU_ACCESS_WRITE,
		0, m_VertexStride
	};

	const D3D11_SUBRESOURCE_DATA srData{ pInitVertices,0,0 };
	const HRESULT hr = Globals::pGpu->GetDevice().CreateBuffer(&vertexBufferDesc, &srData, &m_pVertexBuffer);
	if (FAILED(hr))
		Logger::PrintError("Mesh::InitVertexBuffer");
}

Mesh::~Mesh()
{
	if (m_IndexCount > 0)
		SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}

void Mesh::Activate() const
{
	(this->*m_fpActivate)();
}

void Mesh::Draw() const
{
	(this->*m_fpDraw)();
}

void Mesh::ActivateIndexed() const
{
	Globals::pGpu->GetContext().IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer,
		&m_VertexStride,
		&m_VertexOffset);
	Globals::pGpu->GetContext().IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Globals::pGpu->GetContext().IASetPrimitiveTopology(m_Topology);
}

void Mesh::ActivateUnindexed() const
{
	Globals::pGpu->GetContext().IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer,
		&m_VertexStride,
		&m_VertexOffset);
	Globals::pGpu->GetContext().IASetPrimitiveTopology(m_Topology);
}

void Mesh::DrawIndexed() const
{
	Globals::pGpu->GetContext().DrawIndexed(m_IndexCount, 0, 0);
}

void Mesh::DrawUnindexed() const
{
	Globals::pGpu->GetContext().Draw(m_VertexCount, m_VertexOffset);
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
