#include "pch.h"
#include "IdxBuffer.h"

using namespace Rendering;

IdxBuffer::IdxBuffer()
	: m_pBuffer{ nullptr }
	, m_Capacity{ 0 }
{
}

IdxBuffer::IdxBuffer(PtrRangeConst<int> indices, bool dynamic)
	: IdxBuffer{ indices.pData, indices.count, dynamic }
{
}

IdxBuffer::IdxBuffer(const int* pData, unsigned count, bool dynamic)
	: m_pBuffer{ nullptr }
	, m_Capacity{ count }
{
	Dx::DxHelper::CreateIndexBuffer(m_pBuffer, pData, static_cast<int>(count), !dynamic);
}

IdxBuffer::~IdxBuffer()
{
	if (m_pBuffer)
	{
		m_pBuffer->Release();
		m_pBuffer = nullptr;
		m_Capacity = 0;
	}
}

IdxBuffer::IdxBuffer(IdxBuffer&& other) noexcept
	: m_pBuffer{ other.m_pBuffer }
	, m_Capacity{ other.m_Capacity }
{
	other.m_pBuffer = nullptr;
	other.m_Capacity = 0;
}

IdxBuffer& IdxBuffer::operator=(IdxBuffer&& other) noexcept
{
	if (&other == this) return *this;
	if (m_pBuffer) m_pBuffer->Release();
	m_pBuffer = other.m_pBuffer;
	m_Capacity = other.m_Capacity;
	other.m_pBuffer = nullptr;
	other.m_Capacity = 0;
	return *this;
}

void IdxBuffer::Activate() const
{
	Globals::pGpu->GetContext().IASetIndexBuffer(m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void IdxBuffer::Draw() const
{
	Globals::pGpu->GetContext().DrawIndexed(m_Capacity, 0, 0);
}

void IdxBuffer::Draw(unsigned count) const
{
	Globals::pGpu->GetContext().DrawIndexed(count, 0, 0);
}

int* IdxBuffer::BeginCopyData()
{
	return Dx::DxHelper::StartUpdateBuffer<int>(m_pBuffer);
}

void IdxBuffer::EndCopyData()
{
	Dx::DxHelper::EndUpdateBuffer(m_pBuffer);
}
