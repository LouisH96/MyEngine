#pragma once
#include "InstanceArray.h"

namespace MyEngine
{
	namespace Rendering
	{
		template<typename Vertex, typename Instance>
		class InstanceList final : IDrawData
		{
		public:
			//---| Constructor/Destructor |---
			InstanceList() = default;
			InstanceList(
				const Vertex* pVertices, unsigned nrVertices,
				PrimitiveTopology topology = PrimitiveTopology::TriangleList);
			~InstanceList() override = default;

			//---| Copy/Move |---
			InstanceList(const InstanceList& other) = delete;
			InstanceList& operator=(const InstanceList& other) = delete;
			InstanceList(InstanceList&& other) noexcept;
			InstanceList& operator=(InstanceList&& other) noexcept;

			//---| Functions |---
			void Draw() override;
			void Draw(unsigned count);
			void Add(const Instance& instance);
			void Insert(const Instance& instance, int idx);
			void EnsureCapacity(int minCapacity);
			void SetCount(int count);

			unsigned GetSize() const { return m_CpuInstances.GetSizeU(); }
			Instance& operator[](unsigned idx);
			const Instance& operator[](unsigned idx) const;
			const Instance& First() const { return m_CpuInstances.First(); }
			const Instance& Last() const { return m_CpuInstances.Last(); }
			Instance& First();
			Instance& Last();

		private:
			InstanceArray m_DrawBatch;
			List<Instance> m_CpuInstances{};
			bool m_Changed{ false };
		};

		template <typename Vertex, typename Instance>
		InstanceList<Vertex, Instance>::InstanceList(
			const Vertex* pVertices, unsigned nrVertices,
			PrimitiveTopology topology)
			: m_DrawBatch{ pVertices, nrVertices, sizeof(Instance), 5, true, false, topology }
		{
		}

		template <typename Vertex, typename Instance>
		InstanceList<Vertex, Instance>::InstanceList(InstanceList&& other) noexcept
			: m_DrawBatch{ std::move(other.m_DrawBatch) }
			, m_CpuInstances{ std::move(other.m_CpuInstances) }
			, m_Changed{ other.m_Changed }
		{
			other.m_Changed = true;
		}

		template <typename Vertex, typename Instance>
		InstanceList<Vertex, Instance>& InstanceList<Vertex, Instance>::operator=(InstanceList&& other) noexcept
		{
			if (&other == this) return *this;
			m_DrawBatch = std::move(other.m_DrawBatch);
			m_CpuInstances = std::move(other.m_CpuInstances);
			m_Changed = other.m_Changed;
			other.m_Changed = true;
			return *this;
		}

		template <typename Vertex, typename Instance>
		void InstanceList<Vertex, Instance>::Draw()
		{
			if (m_CpuInstances.IsEmpty()) return;
			if (m_Changed)
			{
				m_Changed = false;
				m_DrawBatch.EnsureInstanceCapacity(m_CpuInstances.GetCapacityU(), false);
				m_DrawBatch.UpdateInstances(m_CpuInstances.GetData(), m_CpuInstances.GetSize());
				m_DrawBatch.SetInstanceCount(m_CpuInstances.GetSize());
			}
			m_DrawBatch.Draw(m_CpuInstances.GetSizeU());
		}

		template <typename Vertex, typename Instance>
		void InstanceList<Vertex, Instance>::Draw(unsigned count)
		{
			if (m_CpuInstances.IsEmpty()) return;
			if (m_Changed)
			{
				m_Changed = false;
				m_DrawBatch.EnsureInstanceCapacity(count, false);
				m_DrawBatch.UpdateInstances(m_CpuInstances.GetData(), count);
			}
			m_DrawBatch.Draw(count);
		}

		template <typename Vertex, typename Instance>
		void InstanceList<Vertex, Instance>::Add(const Instance& instance)
		{
			m_Changed = true;
			m_CpuInstances.Add(instance);
		}

		template <typename Vertex, typename Instance>
		void InstanceList<Vertex, Instance>::Insert(const Instance& instance, int idx)
		{
			m_Changed = true;
			m_CpuInstances.Insert(idx, instance);
		}

		template <typename Vertex, typename Instance>
		void InstanceList<Vertex, Instance>::EnsureCapacity(int minCapacity)
		{
			m_DrawBatch.EnsureInstanceCapacity(minCapacity, false);
		}

		//will take care of capacity
		template <typename Vertex, typename Instance>
		void InstanceList<Vertex, Instance>::SetCount(int count)
		{
			m_Changed = true;
			m_CpuInstances.SetSize(count);
		}

		template <typename Vertex, typename Instance>
		Instance& InstanceList<Vertex, Instance>::operator[](unsigned idx)
		{
			m_Changed = true;
			return m_CpuInstances[idx];
		}

		template <typename Vertex, typename Instance>
		const Instance& InstanceList<Vertex, Instance>::operator[](unsigned idx) const
		{
			return m_CpuInstances[idx];
		}

		template <typename Vertex, typename Instance>
		Instance& InstanceList<Vertex, Instance>::First()
		{
			m_Changed = true;
			return m_CpuInstances.First();
		}

		template <typename Vertex, typename Instance>
		Instance& InstanceList<Vertex, Instance>::Last()
		{
			m_Changed = true;
			return m_CpuInstances.Last();
		}
	}
}
