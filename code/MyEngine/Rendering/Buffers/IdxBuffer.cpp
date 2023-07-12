#include "pch.h"
#include "IdxBuffer.h"

Rendering::IdxBuffer::IdxBuffer()
	: m_pBuffer{ nullptr }
	, m_Capacity{ 0 }
{
}

Rendering::IdxBuffer::IdxBuffer(const int* pData, unsigned count)
	: m_pBuffer{ nullptr }
	, m_Capacity{ count }
{
	Dx::DxHelper::CreateIndexBuffer(m_pBuffer, pData, static_cast<int>(count));
}

Rendering::IdxBuffer::~IdxBuffer()
{
	if (m_pBuffer)
	{
		m_pBuffer->Release();
		m_pBuffer = nullptr;
		m_Capacity = 0;
	}
}

Rendering::IdxBuffer::IdxBuffer(IdxBuffer&& other) noexcept
	: m_pBuffer{ other.m_pBuffer }
	, m_Capacity{ other.m_Capacity }
{
	other.m_pBuffer = nullptr;
	other.m_Capacity = 0;
}

Rendering::IdxBuffer& Rendering::IdxBuffer::operator=(IdxBuffer&& other) noexcept
{
	if (&other == this) return *this;
	if (m_pBuffer) m_pBuffer->Release();
	m_pBuffer = other.m_pBuffer;
	m_Capacity = other.m_Capacity;
	other.m_pBuffer = nullptr;
	other.m_Capacity = 0;
	return *this;
}

void Rendering::IdxBuffer::Activate() const
{
	Globals::pGpu->GetContext().IASetIndexBuffer(m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Rendering::IdxBuffer::Draw() const
{
	Globals::pGpu->GetContext().DrawIndexed(m_Capacity, 0, 0);
}
