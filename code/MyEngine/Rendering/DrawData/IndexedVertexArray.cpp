#include "pch.h"
#include "IndexedVertexArray.h"

Rendering::IndexedVertexArray::~IndexedVertexArray()
{
	if (m_pBuffers[0])m_pBuffers[0]->Release();
	if (m_pBuffers[1])m_pBuffers[1]->Release();
	if (m_pBuffers[2])m_pBuffers[2]->Release();
}

Rendering::IndexedVertexArray::IndexedVertexArray(IndexedVertexArray&& other) noexcept
	: m_pBuffers{ other.m_pBuffers[0], other.m_pBuffers[1],  other.m_pBuffers[2] }
	, m_Strides{ other.m_Strides[0], other.m_Strides[1], other.m_Strides[2] }
	, m_Offsets{ other.m_Offsets[0], other.m_Offsets[1], other.m_Offsets[2] }
	, m_Sizes{ other.m_Sizes[0], other.m_Sizes[1], other.m_Offsets[2] }
	, m_DrawCountInstances{ other.m_DrawCountInstances }
	, m_DrawCountIndices{ other.m_DrawCountIndices }
{
	other.m_pBuffers[0] = nullptr;
	other.m_pBuffers[1] = nullptr;
	other.m_pBuffers[2] = nullptr;
}

Rendering::IndexedVertexArray& Rendering::IndexedVertexArray::operator=(IndexedVertexArray&& other) noexcept
{
	m_pBuffers[0] = other.m_pBuffers[0]; m_pBuffers[1] = other.m_pBuffers[1]; m_pBuffers[2] = other.m_pBuffers[2];
	m_Sizes[0] = other.m_Sizes[0]; m_Sizes[1] = other.m_Sizes[1];  m_Sizes[2] = other.m_Sizes[2];
	m_Strides[0] = other.m_Strides[0]; m_Strides[1] = other.m_Strides[1];  m_Strides[2] = other.m_Strides[2];
	m_Offsets[0] = other.m_Offsets[0]; m_Offsets[1] = other.m_Offsets[1]; m_Offsets[2] = other.m_Offsets[2];
	m_DrawCountIndices = other.m_DrawCountIndices; m_DrawCountInstances = other.m_DrawCountInstances;
	other.m_pBuffers[0] = nullptr; other.m_pBuffers[1] = nullptr; other.m_pBuffers[2] = nullptr;
	return *this;
}

void Rendering::IndexedVertexArray::Draw(const Gpu& gpu)
{
	gpu.GetContext().IASetVertexBuffers(0, 2, m_pBuffers, m_Strides, m_Offsets);
	gpu.GetContext().IASetIndexBuffer(m_pBuffers[IDX_INDICES], DXGI_FORMAT_R32_UINT, m_Offsets[IDX_INDICES]);
	gpu.GetContext().IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gpu.GetContext().DrawIndexedInstanced(m_DrawCountIndices, m_DrawCountInstances, 0, 0, 0);
}

unsigned Rendering::IndexedVertexArray::GetInstancesSize() const
{
	return m_Sizes[IDX_INSTANCES];
}
