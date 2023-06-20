#include "pch.h"
#include "VertexIdxArray.h"

Rendering::VertexIdxArray::VertexIdxArray()
	: m_pBuffers{ nullptr, nullptr }
{
}

Rendering::VertexIdxArray::~VertexIdxArray()
{
	if (m_pBuffers[IDX_VERTICES]) m_pBuffers[IDX_VERTICES];
	if (m_pBuffers[IDX_INDICES]) m_pBuffers[IDX_INDICES];
	m_pBuffers[IDX_VERTICES] = nullptr;
	m_pBuffers[IDX_INDICES] = nullptr;
}

Rendering::VertexIdxArray::VertexIdxArray(unsigned vertexStride, unsigned vertexCapacity, unsigned indexCapacity)
	: m_Strides{ vertexStride, sizeof(int) }
	, m_Offsets{ 0,0 }
	, m_Counts{ 0,0 }
	, m_Capacities{ vertexCapacity, indexCapacity }
{
	Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_VERTICES], vertexStride, static_cast<int>(vertexCapacity), false);
	Dx::DxHelper::CreateIndexBuffer(m_pBuffers[IDX_INDICES], indexCapacity, false);
}

Rendering::VertexIdxArray::VertexIdxArray(VertexIdxArray&& other) noexcept
	: m_pBuffers{ other.m_pBuffers[0], other.m_pBuffers[1] }
	, m_Strides{ other.m_Strides[0], other.m_Strides[1] }
	, m_Offsets{ other.m_Offsets[0], other.m_Offsets[1] }
	, m_Counts{ other.m_Counts[0], other.m_Counts[1] }
	, m_Capacities{ other.m_Capacities[0], other.m_Capacities[1] }
{
	other.m_pBuffers[0] = nullptr;
	other.m_pBuffers[1] = nullptr;
}

Rendering::VertexIdxArray& Rendering::VertexIdxArray::operator=(VertexIdxArray&& other) noexcept
{
	if (&other == this) return *this;
	m_pBuffers[0] = other.m_pBuffers[0]; m_pBuffers[1] = other.m_pBuffers[1];
	m_Counts[0] = other.m_Counts[0]; m_Counts[1] = other.m_Counts[1];
	m_Strides[0] = other.m_Strides[0]; m_Strides[1] = other.m_Strides[1];
	m_Offsets[0] = other.m_Offsets[0]; m_Offsets[1] = other.m_Offsets[1];
	m_Capacities[0] = other.m_Capacities[0]; m_Capacities[1] = other.m_Capacities[1];
	other.m_pBuffers[0] = nullptr; other.m_pBuffers[1] = nullptr;
	return *this;
}

void Rendering::VertexIdxArray::Draw(unsigned idx) const
{
	Globals::pGpu->GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Globals::pGpu->GetContext().IASetIndexBuffer(m_pBuffers[IDX_INDICES], DXGI_FORMAT_R32_UINT, m_Offsets[IDX_INDICES]);
	Globals::pGpu->GetContext().IASetVertexBuffers(idx, 1, m_pBuffers, m_Strides, m_Offsets);
	Globals::pGpu->GetContext().DrawIndexed(m_Counts[IDX_INDICES], 0, 0);
}

void Rendering::VertexIdxArray::SetVertexCount(unsigned count)
{
	m_Counts[IDX_VERTICES] = count;
}

void Rendering::VertexIdxArray::SetIndexCount(unsigned count)
{
	m_Counts[IDX_INDICES] = count;
}

void Rendering::VertexIdxArray::EnsureVertexCapacity(unsigned minCapacity, bool dynamic)
{
	if (m_Capacities[IDX_VERTICES] >= minCapacity) return;
	SetVertexCapacity(minCapacity, dynamic);
}

void Rendering::VertexIdxArray::EnsureIndexCapacity(unsigned minCapacity, bool dynamic)
{
	if (m_Capacities[IDX_INDICES] >= minCapacity) return;
	SetIndexCapacity(minCapacity, dynamic);
}

void Rendering::VertexIdxArray::UpdateIndices(const int* pIndices, unsigned nrIndices)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	const HRESULT result{ Globals::pGpu->GetContext().Map(m_pBuffers[IDX_INDICES], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
	if (FAILED(result)) Logger::PrintError("[VertexIdxArray] Failed updating indices");
	memcpy(mappedResource.pData, pIndices, nrIndices * sizeof(int));
	Globals::pGpu->GetContext().Unmap(m_pBuffers[IDX_INDICES], 0);
}

void Rendering::VertexIdxArray::SetVertexCapacity(unsigned capacity, bool dynamic)
{
	m_pBuffers[IDX_VERTICES]->Release();
	Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_VERTICES], m_Strides[IDX_VERTICES], static_cast<int>(capacity), !dynamic);
	m_Capacities[IDX_VERTICES] = capacity;
}

void Rendering::VertexIdxArray::SetIndexCapacity(unsigned capacity, bool dynamic)
{
	m_pBuffers[IDX_INDICES]->Release();
	Dx::DxHelper::CreateIndexBuffer(m_pBuffers[IDX_INDICES], static_cast<int>(capacity), !dynamic);
	m_Capacities[IDX_INDICES] = capacity;
}
