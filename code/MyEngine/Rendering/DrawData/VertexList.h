#pragma once
#include "VertexArray.h"
#include "DataStructures/List.h"

namespace MyEngine
{
	namespace Rendering
	{
		template<typename Vertex>
		class VertexList
		{
		public:
			//---| Constructor/Destructor |---
			VertexList(int capacity = 5, PrimitiveTopology topology = PrimitiveTopology::TriangleList);
			~VertexList() = default;

			//---| Move/Copy |---
			VertexList(VertexList&& other) noexcept = delete;
			VertexList& operator=(VertexList&& other) noexcept = delete;
			VertexList(const VertexList& other) = default;
			VertexList& operator=(const VertexList& other) = default;

			//---| Functions |---
			void Add(const Vertex& vertex);
			void Clear();
			void Draw();

			int GetSize() const { return m_CpuList.GetSize(); }
			Vertex& operator[](int idx);
			const Vertex& operator[](int idx) const;

			List<Vertex>& GetList();
			const List<Vertex>& GetList() const { return m_CpuList; }

		private:
			List<Vertex> m_CpuList;
			VertexArray m_VertexArray;
			bool m_Changed;
		};

		template <typename Vertex>
		VertexList<Vertex>::VertexList(int capacity, PrimitiveTopology topology)
			: m_CpuList{ capacity }
			, m_VertexArray{ *Globals::pGpu, sizeof(Vertex), capacity, false, topology }
			, m_Changed{ false }
		{
		}

		template <typename Vertex>
		void VertexList<Vertex>::Add(const Vertex& vertex)
		{
			m_CpuList.Add(vertex);
			m_Changed = true;
		}

		template <typename Vertex>
		void VertexList<Vertex>::Clear()
		{
			m_CpuList.Clear();
			m_Changed = true;
		}

		template <typename Vertex>
		void VertexList<Vertex>::Draw()
		{
			if (m_CpuList.IsEmpty()) return;
			if (m_Changed)
			{
				m_VertexArray.SetCount(m_CpuList.GetSize());
				m_VertexArray.EnsureCapacity(*Globals::pGpu, m_CpuList.GetCapacity(), false);
				m_VertexArray.UpdateData(*Globals::pGpu, m_CpuList.GetData(), m_CpuList.GetSize());
				m_Changed = false;
			}
			m_VertexArray.Activate(*Globals::pGpu);
			m_VertexArray.Draw(*Globals::pGpu);
		}

		template <typename Vertex>
		Vertex& VertexList<Vertex>::operator[](int idx)
		{
			m_Changed = true;
			return m_CpuList[idx];
		}

		template <typename Vertex>
		const Vertex& VertexList<Vertex>::operator[](int idx) const
		{
			return m_CpuList[idx];
		}

		template <typename Vertex>
		List<Vertex>& VertexList<Vertex>::GetList()
		{
			m_Changed = true;
			return m_CpuList;
		}
	}
}
