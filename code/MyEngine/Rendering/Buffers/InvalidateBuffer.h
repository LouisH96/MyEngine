#pragma once
#include "Buffer.h"
#include "DataStructures/InvalidateList.h"

namespace MyEngine
{
	namespace Rendering
	{
		template<typename Data>
		class InvalidateBuffer
		{
		public:
			//---| Construction |---
			InvalidateBuffer() = default;
			explicit InvalidateBuffer(unsigned initCapacity);

			//---| Functions |---
			int Add(Data&& data);
			Data Remove(int idx);

			const InvalidateList<Data>& GetCpuData() const { return m_CpuBuffer; }
			InvalidateList<Data>& GetCpuData() { return m_CpuBuffer; }

			unsigned GetCount() const { return m_CpuBuffer.GetSize(); }
			const Data* GetFirst() const { return m_CpuBuffer.GetFirst(); }
			const Data* GetLast() const { return m_CpuBuffer.GetLast(); }
			const Data* GetEnd() const { return m_CpuBuffer.GetEnd(); }
			Data* GetFirst() { return m_CpuBuffer.GetFirst(); }
			Data* GetLast() { return m_CpuBuffer.GetLast(); }
			Data* GetEnd() { return m_CpuBuffer.GetEnd(); }

			const Data& operator[](int idx) const { return m_CpuBuffer.GetData()[idx]; }
			Data& operator[](int idx) { return m_CpuBuffer.GetData()[idx]; }

			void ActivateVertexBuffer(unsigned slot);
			void Draw(unsigned nrVertices) const;
			void DrawIdx(unsigned nrIndices) const;

		private:
			Buffer<Data> m_GpuBuffer;
			InvalidateList<Data> m_CpuBuffer;
		};

		template <typename Data>
		InvalidateBuffer<Data>::InvalidateBuffer(unsigned initCapacity)
			: m_GpuBuffer{ initCapacity, true }
			, m_CpuBuffer{ initCapacity }
		{
		}

		template <typename Data>
		int InvalidateBuffer<Data>::Add(Data&& data)
		{
			return m_CpuBuffer.Add(std::move(data));
		}

		template <typename Data>
		Data InvalidateBuffer<Data>::Remove(int idx)
		{
			return m_CpuBuffer.Remove(idx);
		}

		template <typename Data>
		void InvalidateBuffer<Data>::ActivateVertexBuffer(unsigned slot)
		{
			if (m_CpuBuffer.HasChanged())
			{
				m_CpuBuffer.ClearChangedFlag();
				m_GpuBuffer.EnsureCapacity(m_CpuBuffer.GetCapacity());
				m_GpuBuffer.CopyData(m_CpuBuffer.GetFirst(), m_CpuBuffer.GetSize());
			}
			m_GpuBuffer.Activate(slot);
		}

		template <typename Data>
		void InvalidateBuffer<Data>::Draw(unsigned nrVertices) const
		{
			Globals::pGpu->GetContext().DrawInstanced(nrVertices, GetCount(), 0, m_CpuBuffer.GetFirstIdx());
		}

		template <typename Data>
		void InvalidateBuffer<Data>::DrawIdx(unsigned nrIndices) const
		{
			Globals::pGpu->GetContext().DrawIndexedInstanced(nrIndices, GetCount(), 0, 0, m_CpuBuffer.GetFirstIdx());
		}
	}
}
