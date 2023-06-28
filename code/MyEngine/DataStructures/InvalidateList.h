#pragma once
#define INVALIDATE_LIST_DEBUG

namespace MyEngine
{
	namespace Ds
	{
		template<typename Data, bool IsValid(const Data&), void Invalidate(Data&)>
		class InvalidateList
		{
		public:
			//---| Construction |---
			explicit InvalidateList(unsigned capacity = 4);
			~InvalidateList();

			InvalidateList(const InvalidateList& other);
			InvalidateList& operator=(const InvalidateList& other);

			InvalidateList(InvalidateList&& other) noexcept;
			InvalidateList& operator=(InvalidateList&& other) noexcept;

			//---| Functions |---
			int Add(Data&& data);
			Data Remove(int idx);

			bool HasChanged() const;
			void ClearChangedFlag();

			int GetSize() const { return m_End - m_First; }
			unsigned GetCapacity() const { return m_Capacity; }

			const Data* GetFirst() const;
			Data* GetFirst();

		private:
			Data* m_pData;
			unsigned m_Capacity;
			int m_First, m_End, m_GapIndicator;
			bool m_Changed;

			void IncreaseCapacity(unsigned increase);

			bool IsEmpty(int idx) const;

			void UpdateFirstIndicator();
			void UpdateEndIndicator();
			void UpdateGapIndicator();
		};

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		InvalidateList<Data, IsValid, Invalidate>::InvalidateList(unsigned capacity)
			: m_pData{ new Data[capacity] }
			, m_Capacity{ capacity }
			, m_First{ 0 }, m_End{ 0 }, m_GapIndicator{ 0 }
			, m_Changed{ false }
		{
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		InvalidateList<Data, IsValid, Invalidate>::~InvalidateList()
		{
			delete[] m_pData;
			m_Capacity = 0;
			m_First = 0;
			m_End = 0;
			m_GapIndicator = 0;
			m_Changed = true;
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		InvalidateList<Data, IsValid, Invalidate>::InvalidateList(const InvalidateList& other)
			: m_pData{ new Data[other.m_Capacity] }
			, m_Capacity{ other.m_Capacity }
			, m_First{ other.m_First }
			, m_End{ other.m_End }
			, m_GapIndicator{ other.m_GapIndicator }
			, m_Changed{ false }
		{
			std::copy(&other.m_pData[m_First], &other.m_pData[other.m_End], &m_pData[m_First]);
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		InvalidateList<Data, IsValid, Invalidate>& InvalidateList<Data, IsValid, Invalidate>::operator=(
			const InvalidateList& other)
		{
			if (&other == this) return *this;
			m_Capacity = other.m_Capacity;
			m_First = other.m_First;
			m_End = other.m_End;
			m_GapIndicator = other.m_GapIndicator;
			m_Changed = true;
			delete[] m_pData;
			m_pData = new Data[other.m_Capacity];
			std::copy(&other.m_pData[m_First], &other.m_pData[m_End], &m_pData[m_First]);
			return *this;
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		InvalidateList<Data, IsValid, Invalidate>::InvalidateList(InvalidateList&& other) noexcept
			: m_pData{ other.m_pData }
			, m_Capacity{ other.m_Capacity }
			, m_First{ other.m_First }
			, m_End{ other.m_End }
			, m_GapIndicator{ other.m_GapIndicator }
			, m_Changed{ false }
		{
			other.m_pData = nullptr;
			other.m_Capacity = 0;
			other.m_First = 0;
			other.m_End = 0;
			other.m_GapIndicator = 0;
			other.m_Changed = true;
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		InvalidateList<Data, IsValid, Invalidate>& InvalidateList<Data, IsValid, Invalidate>::operator=(
			InvalidateList&& other) noexcept
		{
			if (&other == this) return *this;
			m_pData = other.m_pData;
			m_Capacity = other.m_Capacity;
			m_First = other.m_First;
			m_End = other.m_End;
			m_GapIndicator = other.m_GapIndicator;
			m_Changed = true;
			other.m_pData = nullptr;
			other.m_Capacity = 0;
			other.m_First = 0;
			other.m_End = 0;
			other.m_GapIndicator = 0;
			other.m_Changed = true;
			return *this;
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		int InvalidateList<Data, IsValid, Invalidate>::Add(Data&& data)
		{
#ifdef INVALIDATE_LIST_DEBUG
			if (!IsEmpty(m_GapIndicator))
				Logger::PrintError("[InvalidateList::Add] GapIndicator is not empty");
#endif
			m_Changed = true;
			const int idx{ m_GapIndicator };
			m_pData[idx] = std::move(data);
			if (idx >= m_End) m_End = idx + 1;
			else if (idx < m_First) m_First = idx;
			UpdateGapIndicator();
			return idx;
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		Data InvalidateList<Data, IsValid, Invalidate>::Remove(int idx)
		{
#ifdef INVALIDATE_LIST_DEBUG
			if (idx == m_GapIndicator)
				Logger::PrintError("[InvalidateList::Remove] idx is same as gap-indicator");
			if (idx >= m_End)
				Logger::PrintError("[InvalidateList::Remove] idx is equal or bigger than end");
#endif
			m_Changed = true;
			const Data removed{ m_pData[idx] };
			Invalidate(m_pData[idx]);

			if (m_End - m_First == 1)
			{
				m_First = 0;
				m_End = 0;
				m_GapIndicator = 0;
				return removed;
			}
			if (idx < m_GapIndicator) m_GapIndicator = idx;
			if (idx == m_First) UpdateFirstIndicator();
			else if (idx == m_End - 1) UpdateEndIndicator();
			return removed;
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		bool InvalidateList<Data, IsValid, Invalidate>::HasChanged() const
		{
			return m_Changed;
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		void InvalidateList<Data, IsValid, Invalidate>::ClearChangedFlag()
		{
			m_Changed = false;
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		const Data* InvalidateList<Data, IsValid, Invalidate>::GetFirst() const
		{
			return m_pData[m_First];
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		Data* InvalidateList<Data, IsValid, Invalidate>::GetFirst()
		{
			m_Changed = true;
			return &m_pData[m_First];
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		bool InvalidateList<Data, IsValid, Invalidate>::IsEmpty(int idx) const
		{
			return !IsValid(m_pData[idx]);
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		void InvalidateList<Data, IsValid, Invalidate>::UpdateFirstIndicator()
		{
#ifdef INVALIDATE_LIST_DEBUG
			if (!IsEmpty(m_First))
				Logger::PrintError("[InvalidateList::UpdateFirstIndicator] expected first to be empty");
#endif
			while (++m_First < m_End)
				if (!IsEmpty(m_First))
					return;
#ifdef INVALIDATE_LIST_DEBUG
			Logger::PrintError("[InvalidateList::UpdateFirstIndicator] didn't expect to reach end");
#endif
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		void InvalidateList<Data, IsValid, Invalidate>::UpdateGapIndicator()
		{
			while (++m_GapIndicator < m_End)
				if (IsEmpty(m_GapIndicator)) return;

			if (m_GapIndicator == m_Capacity)
				IncreaseCapacity(m_Capacity);
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		void InvalidateList<Data, IsValid, Invalidate>::UpdateEndIndicator()
		{
#ifdef INVALIDATE_LIST_DEBUG
			if (!IsEmpty(m_End - 1))
				Logger::PrintError("[InvalidateList::UpdateEndIndicator] shouldn't call this if last isn't empty");
			if (m_First != 0 && IsEmpty(m_First))
				Logger::PrintError("[InvalidateList::UpdateEndIndicator] first should be 0 or not empty");
#endif
			int last{ m_End - 1 };
			while (--last >= m_First)
				if (!IsEmpty(last))
				{
					m_End = last + 1;
					return;
				}

#ifdef INVALIDATE_LIST_DEBUG
			if (last != 0 || m_First != 0)
				Logger::PrintError("[InvalidateList::UpdateEndIndicator] expected last & first to be 0");
#endif
			m_End = 0;
		}

		template <typename Data, bool(*IsValid)(const Data&), void(*Invalidate)(Data&)>
		void InvalidateList<Data, IsValid, Invalidate>::IncreaseCapacity(unsigned increase)
		{
			Data* pNew = new Data[m_Capacity + increase];
			std::move(&m_pData[m_First], &m_pData[m_End], &pNew[m_First]);
			delete[] m_pData;
			m_pData = pNew;
			m_Capacity = m_Capacity + increase;
		}
	}
}
