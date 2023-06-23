#include "pch.h"
#include "VertexArray.h"


Rendering::VertexArray::VertexArray()
	: m_pBuffer{ nullptr }
	, m_Stride{ 0 }
	, m_Offset{ 0 }
	, m_Capacity{ 0 }
	, m_Count{ 0 }
	, m_Topology{ PrimitiveTopologyUtils::ToDx(PrimitiveTopology::Unknown) }
{
}

Rendering::VertexArray::VertexArray(int stride, int initCount, bool immutable,
	PrimitiveTopology topology)
	: m_pBuffer{ nullptr }
	, m_Stride{ static_cast<unsigned>(stride) }
	, m_Offset{ 0 }
	, m_Capacity{ static_cast<unsigned>(initCount) }
	, m_Count{ static_cast<unsigned>(initCount) }
	, m_Topology{ PrimitiveTopologyUtils::ToDx(topology) }
{
	Dx::DxHelper::CreateVertexBuffer<char>(m_pBuffer, initCount * stride, immutable);
}

MyEngine::Rendering::VertexArray::~VertexArray()
{
	if (m_pBuffer) m_pBuffer->Release();
	m_pBuffer = nullptr;
}

Rendering::VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_pBuffer{ other.m_pBuffer }
	, m_Stride{ other.m_Stride }
	, m_Offset{ other.m_Offset }
	, m_Capacity{ other.m_Capacity }
	, m_Count{ other.m_Count }
	, m_Topology{ other.m_Topology }
{
	other.m_pBuffer = nullptr;
}

Rendering::VertexArray& Rendering::VertexArray::operator=(VertexArray&& other) noexcept
{
	if (&other == this) return *this;
	m_pBuffer = other.m_pBuffer;
	m_Stride = other.m_Stride;
	m_Offset = other.m_Offset;
	m_Capacity = other.m_Capacity;
	m_Count = other.m_Count;
	m_Topology = other.m_Topology;
	other.m_pBuffer = nullptr;
	return *this;
}

void Rendering::VertexArray::Activate() const
{
	Globals::pGpu->GetContext().IASetPrimitiveTopology(m_Topology);
	Globals::pGpu->GetContext().IASetVertexBuffers(0, 1, &m_pBuffer, &m_Stride, &m_Offset);
}

void Rendering::VertexArray::Activate(int idx) const
{
	Globals::pGpu->GetContext().IASetPrimitiveTopology(m_Topology);
	Globals::pGpu->GetContext().IASetVertexBuffers(idx, 1, &m_pBuffer, &m_Stride, &m_Offset);
}

void Rendering::VertexArray::Draw() const
{
	Globals::pGpu->GetContext().Draw(m_Count, 0);
}

void Rendering::VertexArray::Draw(unsigned offset, unsigned count) const
{
	Globals::pGpu->GetContext().Draw(count, offset);
}

void Rendering::VertexArray::SetOffset(unsigned offset)
{
	m_Offset = offset;
}

void Rendering::VertexArray::SetCapacity(unsigned capacity, bool immutable)
{
	if (m_Capacity == capacity) return;
	m_pBuffer->Release();
	Dx::DxHelper::CreateVertexBuffer(m_pBuffer, m_Stride, capacity, immutable);
	m_Capacity = capacity;
}

void Rendering::VertexArray::SetCount(unsigned count)
{
	m_Count = count;
}

void Rendering::VertexArray::EnsureCapacity(unsigned minCapacity, bool immutable)
{
	if (m_Capacity >= minCapacity) return;
	SetCapacity(minCapacity, immutable);
}
