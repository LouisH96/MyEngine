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
			void Clear();

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
		void VertexIdxList<Vertex>::Draw(unsigned idx)
		{
			if (m_CpuVertices.IsEmpty() || m_CpuIndices.IsEmpty()) return;
			if (m_Changed)
			{
				m_Changed = false;
				m_GpuData.EnsureVertexCapacity(m_CpuVertices.GetSizeU(), true);
				m_GpuData.EnsureIndexCapacity(m_CpuIndices.GetSizeU(), true);

				m_GpuData.UpdateVertices(m_CpuVertices.GetData(), m_CpuVertices.GetSizeU());
				m_GpuData.UpdateIndices(m_CpuIndices.GetData(), m_CpuIndices.GetSizeU());

				m_GpuData.SetVertexCount(m_CpuVertices.GetSizeU());
				m_GpuData.SetIndexCount(m_CpuIndices.GetSizeU());
			}
			m_GpuData.Draw();
		}

		template <typename Vertex>
		void VertexIdxList<Vertex>::Add(const Vertex* pVertices, unsigned nrVertices, const int* pIndices,
			unsigned nrIndices)
		{
			m_Changed = true;
			const unsigned oldSize{ m_CpuVertices.GetSizeU() };
			m_CpuVertices.Add(pVertices, nrVertices);
			m_CpuIndices.EnsureCapacity(m_CpuIndices.GetSizeU() + nrIndices);
			for (int i = 0; i < nrIndices; i++)
				m_CpuIndices.Add(pIndices[i] + oldSize);
		}

		template <typename Vertex>
		void VertexIdxList<Vertex>::Clear()
		{
			m_Changed = true;
			m_CpuVertices.Clear();
			m_CpuIndices.Clear();
		}
	}
}
