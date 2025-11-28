#pragma once
#include "VertexIdxArray.h"
#include "DataStructures/List.h"

struct ID3D11Buffer;

namespace MyEngine
{
	namespace Rendering
	{
		template<typename Vertex>
		class VertexIdxList
		{
		public:
			//---| Constructor/Destructor |---
			VertexIdxList();
			~VertexIdxList() = default;

			VertexIdxList(
				const Vertex* pVertices, unsigned nrVertices,
				const int* pIndices, unsigned nrIndices);

			//---| Copy/Move |---
			VertexIdxList(const VertexIdxList& other) = delete;
			VertexIdxList& operator=(const VertexIdxList& other) = delete;
			VertexIdxList(VertexIdxList&& other) noexcept = default;
			VertexIdxList& operator=(VertexIdxList&& other) noexcept = default;

			//---| Functions |---
			void Draw(unsigned idx = 0);

			void Add(
				const Vertex* pVertices, unsigned nrVertices,
				const int* pIndices, unsigned nrIndices);
			void AddVertex(const Vertex& vertex);
			void AddIndex(unsigned index);
			void AddIndices(unsigned i0, unsigned i1, unsigned i2);

			void Clear();

			List<Vertex>& GetVertices();
			List<int>& GetIndices();
			
			const List<Vertex>& GetVertices() const { return m_CpuVertices; }
			const List<int>& GetIndices() const { return m_CpuIndices; };
			unsigned GetNrVertices() const { return m_CpuVertices.GetSize(); }
			unsigned GetNrIndices() const { return m_CpuIndices.GetSize(); }

		private:
			List<Vertex> m_CpuVertices;
			List<int> m_CpuIndices;
			VertexIdxArray m_GpuData;
			bool m_Changed{ false };
		};

		template <typename Vertex>
		VertexIdxList<Vertex>::VertexIdxList()
			: m_GpuData{ sizeof(Vertex),5,5 }
		{
		}

		template <typename Vertex>
		VertexIdxList<Vertex>::VertexIdxList(const Vertex* pVertices, unsigned nrVertices,
			const int* pIndices, unsigned nrIndices)
			: m_GpuData{ pVertices, nrVertices, true, pIndices, nrIndices, true }
		{
			m_CpuVertices.Add(pVertices, nrVertices);
			m_CpuIndices.Add(pIndices, nrIndices);
		}

		template <typename Vertex>
		void VertexIdxList<Vertex>::Draw(unsigned)
		{
			if (m_CpuVertices.IsEmpty() || m_CpuIndices.IsEmpty()) return;
			if (m_Changed)
			{
				m_Changed = false;
				m_GpuData.EnsureVertexCapacity(m_CpuVertices.GetSize(), true);
				m_GpuData.EnsureIndexCapacity(m_CpuIndices.GetSize(), true);

				m_GpuData.UpdateVertices(m_CpuVertices.GetData(), m_CpuVertices.GetSize());
				m_GpuData.UpdateIndices(m_CpuIndices.GetData(), m_CpuIndices.GetSize());

				m_GpuData.SetVertexCount(m_CpuVertices.GetSize());
				m_GpuData.SetIndexCount(m_CpuIndices.GetSize());
			}
			m_GpuData.Draw();
		}

		template <typename Vertex>
		void VertexIdxList<Vertex>::Add(const Vertex* pVertices, unsigned nrVertices, const int* pIndices,
			unsigned nrIndices)
		{
			m_Changed = true;
			const unsigned oldSize{ m_CpuVertices.GetSize() };
			m_CpuVertices.Add(pVertices, nrVertices);
			m_CpuIndices.EnsureCapacity(m_CpuIndices.GetSize() + nrIndices);
			for (unsigned i = 0; i < nrIndices; i++)
				m_CpuIndices.Add(pIndices[i] + oldSize);
		}

		template<typename Vertex>
		inline void VertexIdxList<Vertex>::AddVertex(const Vertex& vertex)
		{
			m_Changed = true;
			m_CpuVertices.Add(vertex);
		}

		template<typename Vertex>
		inline void VertexIdxList<Vertex>::AddIndex(unsigned index)
		{
			m_Changed = true;
			m_CpuIndices.Add(index);
		}

		template<typename Vertex>
		inline void VertexIdxList<Vertex>::AddIndices(unsigned i0, unsigned i1, unsigned i2)
		{
			m_Changed = true;
			m_CpuIndices.Add(i0, i1, i2);
		}

		template <typename Vertex>
		void VertexIdxList<Vertex>::Clear()
		{
			m_Changed = true;
			m_CpuVertices.Clear();
			m_CpuIndices.Clear();
		}
		template<typename Vertex>
		inline List<Vertex>& VertexIdxList<Vertex>::GetVertices()
		{
			m_Changed = true;
			return m_CpuVertices;
		}
		template<typename Vertex>
		inline List<int>& VertexIdxList<Vertex>::GetIndices()
		{
			m_Changed = true;
			return m_CpuIndices;
		}
	}
}
