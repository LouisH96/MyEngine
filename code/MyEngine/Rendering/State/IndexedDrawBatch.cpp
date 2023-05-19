#include "pch.h"
#include "IndexedDrawBatch.h"

Rendering::IndexedDrawBatch::~IndexedDrawBatch()
{
	if (m_pBuffers[0])m_pBuffers[0]->Release();
	if (m_pBuffers[1])m_pBuffers[1]->Release();
	if (m_pBuffers[2])m_pBuffers[2]->Release();
}

Rendering::IndexedDrawBatch::IndexedDrawBatch(IndexedDrawBatch&& other) noexcept
	: m_pBuffers{ other.m_pBuffers[0], other.m_pBuffers[1],  other.m_pBuffers[2] }
	, m_Strides{ other.m_Strides[0], other.m_Strides[1], other.m_Strides[2] }
	, m_Offsets{ other.m_Offsets[0], other.m_Offsets[1], other.m_Offsets[2] }
	, m_Counts{ other.m_Counts[0], other.m_Counts[1], other.m_Offsets[2] }
{
	other.m_pBuffers[0] = nullptr;
	other.m_pBuffers[1] = nullptr;
	other.m_pBuffers[2] = nullptr;
}

Rendering::IndexedDrawBatch& Rendering::IndexedDrawBatch::operator=(IndexedDrawBatch&& other) noexcept
{
	m_pBuffers[0] = other.m_pBuffers[0]; m_pBuffers[1] = other.m_pBuffers[1]; m_pBuffers[2] = other.m_pBuffers[2];
	m_Counts[0] = other.m_Counts[0]; m_Counts[1] = other.m_Counts[1];  m_Counts[2] = other.m_Counts[2];
	m_Strides[0] = other.m_Strides[0]; m_Strides[1] = other.m_Strides[1];  m_Strides[2] = other.m_Strides[2];
	m_Offsets[0] = other.m_Offsets[0]; m_Offsets[1] = other.m_Offsets[1]; m_Offsets[2] = other.m_Offsets[2];
	other.m_pBuffers[0] = nullptr; other.m_pBuffers[1] = nullptr; other.m_pBuffers[2] = nullptr;
	return *this;
}

void Rendering::IndexedDrawBatch::Draw(const Gpu& gpu)
{
	gpu.GetContext().IASetVertexBuffers(0, 2, m_pBuffers, m_Strides, m_Offsets);
	gpu.GetContext().IASetIndexBuffer(m_pBuffers[IDX_INDICES], DXGI_FORMAT_R32_UINT, 0);
	gpu.GetContext().IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gpu.GetContext().DrawIndexedInstanced(m_Counts[IDX_INDICES], m_Counts[IDX_INSTANCES], 0,0,0);
}
