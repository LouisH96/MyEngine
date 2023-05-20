#include "pch.h"
#include "VertexBuffer.h"


Rendering::VertexBuffer::VertexBuffer(const Gpu& gpu, int stride, int initCount, bool immutable,
	PrimitiveTopology topology)
	: m_pBuffer{ nullptr }
	, m_Stride{ static_cast<unsigned>(stride)}
	, m_Offset{ 0 }
	, m_Capacity{ static_cast<unsigned>(initCount) }
	, m_Count{ static_cast<unsigned>(initCount) }
	, m_Topology{ PrimitiveTopologyUtils::ToDx(topology) }
{
	Dx::DxHelper::CreateVertexBuffer<char>(gpu, m_pBuffer, initCount * stride, immutable);
}

MyEngine::Rendering::VertexBuffer::~VertexBuffer()
{
	if (m_pBuffer) m_pBuffer->Release();
}

Rendering::VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_pBuffer{ other.m_pBuffer }
	, m_Stride{ other.m_Stride }
	, m_Offset{ other.m_Offset }
	, m_Capacity{ other.m_Capacity }
	, m_Count{ other.m_Count }
	, m_Topology{ other.m_Topology }
{
	other.m_pBuffer = nullptr;
}

Rendering::VertexBuffer& Rendering::VertexBuffer::operator=(VertexBuffer&& other) noexcept
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

void Rendering::VertexBuffer::Activate(const Gpu& gpu) const
{
	gpu.GetContext().IASetPrimitiveTopology(m_Topology);
	gpu.GetContext().IASetVertexBuffers(0, 1, &m_pBuffer, &m_Stride, &m_Offset);
}

void Rendering::VertexBuffer::Draw(const Gpu& gpu) const
{
	gpu.GetContext().Draw(m_Count, 0);
}

void Rendering::VertexBuffer::SetOffset(unsigned offset)
{
	m_Offset = offset;
}

void Rendering::VertexBuffer::SetCapacity(const Gpu& gpu, unsigned capacity, bool immutable)
{
	if (m_Capacity == capacity) return;
	m_pBuffer->Release();
	Dx::DxHelper::CreateVertexBuffer(gpu, m_pBuffer, m_Stride, capacity, immutable);
	m_Capacity = capacity;
}

void Rendering::VertexBuffer::SetCount(unsigned count)
{
	m_Count = count;
}

void Rendering::VertexBuffer::EnsureCapacity(const Gpu& gpu, unsigned minCapacity, bool immutable)
{
	if (m_Capacity >= minCapacity) return;
	SetCapacity(gpu, minCapacity, immutable);
}
