#include "pch.h"
#include "InstanceIdxArray.h"

Rendering::InstanceIdxArray::InstanceIdxArray()
	: m_pBuffers{ nullptr, nullptr, nullptr }
{
}

Rendering::InstanceIdxArray::~InstanceIdxArray()
{
	for (auto pBuffer : m_pBuffers)
		if (pBuffer)
		{
			pBuffer->Release();
			pBuffer = nullptr;
		}
}

Rendering::InstanceIdxArray::InstanceIdxArray(InstanceIdxArray&& other) noexcept
	: m_pBuffers{ other.m_pBuffers[0], other.m_pBuffers[1], other.m_pBuffers[2] }
	, m_Strides{ other.m_Strides[0], other.m_Strides[1], other.m_Strides[2] }
	, m_Offsets{ other.m_Offsets[0], other.m_Offsets[1], other.m_Offsets[2] }
	, m_Counts{ other.m_Counts[0], other.m_Counts[1], other.m_Counts[2] }
	, m_Capacities{ other.m_Capacities[0], other.m_Capacities[1], other.m_Capacities[2] }
{
	other.m_pBuffers[0] = nullptr;
	other.m_pBuffers[1] = nullptr;
	other.m_pBuffers[2] = nullptr;
}

Rendering::InstanceIdxArray& Rendering::InstanceIdxArray::operator=(InstanceIdxArray&& other) noexcept
{
	if (&other == this) return *this;
	m_pBuffers[0] = other.m_pBuffers[0]; m_pBuffers[1] = other.m_pBuffers[1]; m_pBuffers[2] = other.m_pBuffers[2];
	m_Counts[0] = other.m_Counts[0]; m_Counts[1] = other.m_Counts[1]; m_Counts[2] = other.m_Counts[2];
	m_Strides[0] = other.m_Strides[0]; m_Strides[1] = other.m_Strides[1]; m_Strides[2] = other.m_Strides[2];
	m_Offsets[0] = other.m_Offsets[0]; m_Offsets[1] = other.m_Offsets[1]; m_Offsets[2] = other.m_Offsets[2];
	m_Capacities[0] = other.m_Capacities[0]; m_Capacities[1] = other.m_Capacities[1]; m_Capacities[2] = other.m_Capacities[2];
	other.m_pBuffers[0] = nullptr; other.m_pBuffers[1] = nullptr; other.m_pBuffers[2] = nullptr;
	return *this;
}

void Rendering::InstanceIdxArray::Draw() const
{
	Globals::pGpu->GetContext().IASetVertexBuffers(0, 2, m_pBuffers, m_Strides, m_Offsets);
	Globals::pGpu->GetContext().IASetIndexBuffer(m_pBuffers[IDX_INDICES], DXGI_FORMAT_R32_UINT, m_Offsets[IDX_INDICES]);
	Globals::pGpu->GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Globals::pGpu->GetContext().DrawIndexedInstanced(m_Counts[IDX_INDICES], m_Counts[IDX_INSTANCES], m_Offsets[IDX_INDICES], static_cast<int>(m_Offsets[IDX_VERTICES]), m_Offsets[IDX_INSTANCES]);
}

void Rendering::InstanceIdxArray::Draw(unsigned count) const
{
	Globals::pGpu->GetContext().IASetVertexBuffers(0, 2, m_pBuffers, m_Strides, m_Offsets);
	Globals::pGpu->GetContext().IASetIndexBuffer(m_pBuffers[IDX_INDICES], DXGI_FORMAT_R32_UINT, m_Offsets[IDX_INDICES]);
	Globals::pGpu->GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Globals::pGpu->GetContext().DrawIndexedInstanced(m_Counts[IDX_INDICES], count, m_Offsets[IDX_INDICES], static_cast<int>(m_Offsets[IDX_VERTICES]), m_Offsets[IDX_INSTANCES]);
}

void Rendering::InstanceIdxArray::EnsureCapacity(unsigned minCapacity, bool dynamic)
{
	if (m_Capacities[IDX_INSTANCES] >= minCapacity) return;
	SetInstanceCapacity(minCapacity, dynamic);
}

void Rendering::InstanceIdxArray::SetInstancesCount(unsigned count)
{
	m_Counts[IDX_INSTANCES] = count;
}

void Rendering::InstanceIdxArray::SetInstanceCapacity(unsigned capacity, bool dynamic)
{
	m_pBuffers[IDX_INSTANCES]->Release();
	Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_INSTANCES], m_Strides[IDX_INSTANCES], static_cast<int>(capacity), !dynamic);
	m_Capacities[IDX_INSTANCES] = capacity;
}
