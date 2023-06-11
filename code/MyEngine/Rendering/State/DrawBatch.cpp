#include "pch.h"
#include "DrawBatch.h"

Rendering::DrawBatch::~DrawBatch()
{
	if (m_pBuffers[0]) m_pBuffers[0]->Release();
	if (m_pBuffers[1]) m_pBuffers[1]->Release();
}

Rendering::DrawBatch::DrawBatch(DrawBatch&& other) noexcept
	: m_pBuffers{ other.m_pBuffers[0], other.m_pBuffers[1] }
	, m_Strides{ other.m_Strides[0], other.m_Strides[1] }
	, m_Offsets{ other.m_Offsets[0], other.m_Offsets[1] }
	, m_Counts{ other.m_Counts[0], other.m_Counts[1] }
{
	other.m_pBuffers[0] = nullptr;
	other.m_pBuffers[1] = nullptr;
}

Rendering::DrawBatch& Rendering::DrawBatch::operator=(DrawBatch&& other) noexcept
{
	if (&other == this) return *this;
	m_pBuffers[0] = other.m_pBuffers[0]; m_pBuffers[1] = other.m_pBuffers[1];
	m_Counts[0] = other.m_Counts[0]; m_Counts[1] = other.m_Counts[1];
	m_Strides[0] = other.m_Strides[0]; m_Strides[1] = other.m_Strides[1];
	m_Offsets[0] = other.m_Offsets[0]; m_Offsets[1] = other.m_Offsets[1];
	other.m_pBuffers[0] = nullptr; other.m_pBuffers[1] = nullptr;
	return *this;
}

void Rendering::DrawBatch::Draw(const Gpu& gpu)
{
	gpu.GetContext().IASetVertexBuffers(0, 2, m_pBuffers, m_Strides, m_Offsets);
	gpu.GetContext().IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gpu.GetContext().DrawInstanced(m_Counts[IDX_VERTICES], m_Counts[IDX_INSTANCES], 0, 0);
}

void Rendering::DrawBatch::Draw(unsigned instanceCount) const
{
	Globals::pGpu->GetContext().IASetVertexBuffers(0, 2, m_pBuffers, m_Strides, m_Offsets);
	Globals::pGpu->GetContext().IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Globals::pGpu->GetContext().DrawInstanced(m_Counts[IDX_VERTICES], instanceCount, 0, 0);
}
