#pragma once
#include "InstanceIdxArray.h"

namespace MyEngine
{
	namespace Rendering
	{
		template<typename Vertex, typename Instance>
		class InstanceIdxList
		{
		public:
			//---| Constructor/Destructor |---
			InstanceIdxList() = default;
			InstanceIdxList(const Vertex* pVertices, unsigned nrVertices, const int* pIndices, unsigned nrIndices);
			~InstanceIdxList() = default;

			//--| Copy/Move |---
			InstanceIdxList(const InstanceIdxList& other) = delete;
			InstanceIdxList(InstanceIdxList&& other) noexcept;
			InstanceIdxList& operator=(const InstanceIdxList& other) = delete;
			InstanceIdxList& operator=(InstanceIdxList&& other) noexcept;

			//---| Functions |---
			void Draw();
			void Add(const Instance& instance);
			void Insert(const Instance& instance, int idx);
			void EnsureCapacity(int minCapacity);
			void SetCount(int count);
			bool IsEmpty() const;
			void Clear();
			int GetSize() const { return m_CpuInstances.GetSize(); }
			unsigned GetSizeU() const { return m_CpuInstances.GetSizeU(); }

			Instance& operator[](unsigned idx);
			const Instance& operator[](unsigned idx) const;
			const Instance& First() const { return m_CpuInstances.First(); }
			const Instance& Last() const { return m_CpuInstances.Last(); }
			Instance& First();
			Instance& Last();

		private:
			InstanceIdxArray m_GpuInstances;
			List<Instance> m_CpuInstances;
			bool m_Changed{ false };
		};

		template <typename Vertex, typename Instance>
		InstanceIdxList<Vertex, Instance>::InstanceIdxList(const Vertex* pVertices, unsigned nrVertices,
			const int* pIndices, unsigned nrIndices)
			: m_GpuInstances{ pVertices, nrVertices, pIndices, nrIndices, sizeof(Instance), 5, true }
		{
		}

		template <typename Vertex, typename Instance>
		InstanceIdxList<Vertex, Instance>::InstanceIdxList(InstanceIdxList&& other) noexcept
			: m_GpuInstances{ std::move(other.m_GpuInstances) }
			, m_CpuInstances{ std::move(other.m_CpuInstances) }
			, m_Changed{ other.m_Changed }
		{
			other.m_Changed = true;
		}

		template <typename Vertex, typename Instance>
		InstanceIdxList<Vertex, Instance>& InstanceIdxList<Vertex, Instance>::operator=(InstanceIdxList&& other) noexcept
		{
			if (&other == this) return *this;
			m_GpuInstances = std::move(other.m_GpuInstances);
			m_CpuInstances = std::move(other.m_CpuInstances);
			m_Changed = other.m_Changed;
			other.m_Changed = true;
			return *this;
		}

		template <typename Vertex, typename Instance>
		void InstanceIdxList<Vertex, Instance>::Draw()
		{
			if (m_CpuInstances.IsEmpty())return;
			if (m_Changed)
			{
				m_Changed = false;
				m_GpuInstances.EnsureCapacity(m_CpuInstances.GetCapacityU(), true);
				m_GpuInstances.SetInstancesCount(m_CpuInstances.GetSize());
				m_GpuInstances.UpdateInstances(m_CpuInstances.GetData(), m_CpuInstances.GetSizeU());
			}
			m_GpuInstances.Draw(m_CpuInstances.GetSizeU());
		}

		template <typename Vertex, typename Instance>
		void InstanceIdxList<Vertex, Instance>::Add(const Instance& instance)
		{
			m_Changed = true;
			m_CpuInstances.Add(instance);
		}

		template <typename Vertex, typename Instance>
		void InstanceIdxList<Vertex, Instance>::Insert(const Instance& instance, int idx)
		{
			m_Changed = true;
			m_CpuInstances.Insert(instance, idx);
		}

		template <typename Vertex, typename Instance>
		void InstanceIdxList<Vertex, Instance>::EnsureCapacity(int minCapacity)
		{
			m_CpuInstances.EnsureCapacity(minCapacity);
		}

		template <typename Vertex, typename Instance>
		void InstanceIdxList<Vertex, Instance>::SetCount(int count)
		{
			m_Changed = true;
			m_CpuInstances.SetSize(count);
		}

		template <typename Vertex, typename Instance>
		bool InstanceIdxList<Vertex, Instance>::IsEmpty() const
		{
			return m_CpuInstances.IsEmpty();
		}

		template <typename Vertex, typename Instance>
		void InstanceIdxList<Vertex, Instance>::Clear()
		{
			m_Changed = true;
			m_CpuInstances.Clear();
		}

		template <typename Vertex, typename Instance>
		Instance& InstanceIdxList<Vertex, Instance>::operator[](unsigned idx)
		{
			m_Changed = true;
			return m_CpuInstances[idx];
		}

		template <typename Vertex, typename Instance>
		const Instance& InstanceIdxList<Vertex, Instance>::operator[](unsigned idx) const
		{
			return m_CpuInstances[idx];
		}

		template <typename Vertex, typename Instance>
		Instance& InstanceIdxList<Vertex, Instance>::First()
		{
			m_Changed = true;
			return m_CpuInstances.First();
		}

		template <typename Vertex, typename Instance>
		Instance& InstanceIdxList<Vertex, Instance>::Last()
		{
			m_Changed = true;
			return m_CpuInstances.Last();
		}
	}
}
