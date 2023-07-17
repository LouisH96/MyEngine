#pragma once
#include "Rendering/Dx/DxHelper.h"

struct ID3D11Buffer;

namespace MyEngine
{
	namespace Rendering
	{
		template<typename Data>
		class Buffer
		{
		public:
			//---| Construction |---
			Buffer();
			explicit Buffer(PtrRangeConst<Data> data, bool dynamic);
			explicit Buffer(unsigned initCapacity, bool dynamic = true);
			explicit Buffer(const Data* pData, unsigned count, bool dynamic = true);
			~Buffer();

			Buffer(const Buffer& other) = delete;
			Buffer& operator=(const Buffer& other) = delete;

			Buffer(Buffer&& other) noexcept;
			Buffer& operator=(Buffer&& other) noexcept;

			//---| Functions |---
			void EnsureCapacity(unsigned minCapacity);
			void CopyData(PtrRangeConst<Data> data);
			void CopyData(const Data* pData, unsigned count);
			void Activate(unsigned slot) const;
			unsigned GetCapacity() const { return m_Capacity; }
			void Draw();

		private:
			ID3D11Buffer* m_pBuffer;
			unsigned m_Capacity;
			bool m_IsDynamic;
		};

		template <typename Data>
		Buffer<Data>::Buffer()
			: m_pBuffer{ nullptr }
			, m_Capacity(0)
			, m_IsDynamic(false)
		{
		}

		template <typename Data>
		Buffer<Data>::Buffer(PtrRangeConst<Data> data, bool dynamic)
			: Buffer{ data.pData, data.count, dynamic }
		{
		}

		template <typename Data>
		Buffer<Data>::Buffer(unsigned initCapacity, bool dynamic)
			: m_pBuffer{ nullptr }
			, m_Capacity{ initCapacity }
			, m_IsDynamic{ dynamic }
		{
			Dx::DxHelper::CreateVertexBuffer(m_pBuffer, sizeof(Data), m_Capacity, !m_IsDynamic);
		}

		template <typename Data>
		Buffer<Data>::Buffer(const Data* pData, unsigned count, bool dynamic)
			: m_pBuffer{ nullptr }
			, m_Capacity{ count }
			, m_IsDynamic{ dynamic }
		{
			Dx::DxHelper::CreateVertexBuffer(m_pBuffer, pData, static_cast<int>(count), !dynamic);
		}

		template <typename Data>
		Buffer<Data>::~Buffer()
		{
			if (m_pBuffer)
			{
				m_pBuffer->Release();
				m_pBuffer = nullptr;
			}
			m_Capacity = 0;
		}

		template <typename Data>
		Buffer<Data>::Buffer(Buffer&& other) noexcept
			: m_pBuffer{ other.m_pBuffer }
			, m_Capacity{ other.m_Capacity }
			, m_IsDynamic{ other.m_IsDynamic }
		{
			other.m_pBuffer = nullptr;
			other.m_Capacity = 0;
		}

		template <typename Data>
		Buffer<Data>& Buffer<Data>::operator=(Buffer&& other) noexcept
		{
			if (&other == this) return *this;
			if (m_pBuffer) m_pBuffer->Release();
			m_pBuffer = other.m_pBuffer;
			m_Capacity = other.m_Capacity;
			m_IsDynamic = other.m_IsDynamic;
			other.m_pBuffer = nullptr;
			other.m_Capacity = 0;
			return*this;
		}

		template <typename Data>
		void Buffer<Data>::EnsureCapacity(unsigned minCapacity)
		{
			if (minCapacity <= m_Capacity) return;
			m_pBuffer->Release();
			Dx::DxHelper::CreateVertexBuffer(m_pBuffer, sizeof(Data), minCapacity, !m_IsDynamic);
		}

		template <typename Data>
		void Buffer<Data>::CopyData(PtrRangeConst<Data> data)
		{
			CopyData(data.pData, data.count);
		}

		template <typename Data>
		void Buffer<Data>::CopyData(const Data* pData, unsigned count)
		{
			Dx::DxHelper::UpdateBuffer(m_pBuffer, pData, count);
		}

		template <typename Data>
		void Buffer<Data>::Activate(unsigned slot) const
		{
			const unsigned stride{ sizeof(Data) };
			constexpr unsigned offset{ 0 }; //todo: required?
			Globals::pGpu->GetContext().IASetVertexBuffers(slot, 1, &m_pBuffer, &stride, &offset);
		}

		template <typename Data>
		void Buffer<Data>::Draw()
		{
			Globals::pGpu->GetContext().Draw(m_Capacity, 0);
		}
	}
}