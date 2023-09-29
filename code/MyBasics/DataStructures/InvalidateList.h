#pragma once
#define INVALIDATE_LIST_DEBUG

namespace MyEngine
{
	template<typename Data>
	class InvalidateList
	{
	public:
		//---| Construction |---
		explicit InvalidateList(unsigned capacity = 4);
		explicit InvalidateList(const PtrRangeConst<Data>& range);
		~InvalidateList();

		InvalidateList(const InvalidateList& other);
		InvalidateList& operator=(const InvalidateList& other);

		InvalidateList(InvalidateList&& other) noexcept;
		InvalidateList& operator=(InvalidateList&& other) noexcept;

		//---| Functions |---
		int Add(const Data& data);
		int Add(Data&& data);
		int Validate(Data*& pOut);
		Data Remove(int idx);
		Data InvalidateAndReturn(int idx); //same are removing but doesn't deconstruct(move) the object
		void Invalidate(int idx);

		void Clear();

		bool HasChanged() const;
		void ClearChangedFlag();

		unsigned GetSize() const { return m_End - m_First; }
		unsigned GetCapacity() const { return m_Capacity; }

		const Data* GetData() const;
		const Data* GetFirst() const;
		const Data* GetLast() const;
		const Data* GetEnd() const;
		Data* GetFirst();
		Data* GetLast();
		Data* GetEnd();
		Data* GetData();

		const Data* SeeData() const;
		const Data* SeeFirst() const;
		const Data* SeeLast() const;
		const Data* SeeEnd() const;

		const Data& Get(int idx) const;
		Data& Get(int idx);
		const Data& GetSinceStart(unsigned idx) const;
		Data& GetSinceStart(unsigned idx);

		int GetFirstIdx() const { return m_First; }
		int GetLastIdx() const { return m_End - 1; }
		int GetEndIdx() const { return m_End; }

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

		int InternalPreAdd();
		void InternalPostAdd();
	};

	template <typename Data>
	InvalidateList<Data>::InvalidateList(unsigned capacity)
		: m_pData{ new Data[capacity] }
		, m_Capacity{ capacity }
		, m_First{ 0 }, m_End{ 0 }, m_GapIndicator{ 0 }
		, m_Changed{ false }
	{
		for (unsigned i = 0; i < capacity; i++)
			m_pData[i].Invalidate();
	}

	template <typename Data>
	InvalidateList<Data>::InvalidateList(const PtrRangeConst<Data>& range)
		: m_pData{ new Data[range.count] }
		, m_Capacity{ range.count }
		, m_First{ 0 }, m_End{ 0 }, m_GapIndicator{ 0 }
		, m_Changed{ true }
	{
		std::copy(range.First(), range.End(), m_pData);
	}

	template <typename Data>
	InvalidateList<Data>::~InvalidateList()
	{
		delete[] m_pData;
		m_Capacity = 0;
		m_First = 0;
		m_End = 0;
		m_GapIndicator = 0;
		m_Changed = true;
	}

	template <typename Data>
	InvalidateList<Data>::InvalidateList(const InvalidateList& other)
		: m_pData{ new Data[other.m_Capacity] }
		, m_Capacity{ other.m_Capacity }
		, m_First{ other.m_First }
		, m_End{ other.m_End }
		, m_GapIndicator{ other.m_GapIndicator }
		, m_Changed{ false }
	{
		std::copy(&other.m_pData[m_First], &other.m_pData[other.m_End], &m_pData[m_First]);
	}

	template <typename Data>
	InvalidateList<Data>& InvalidateList<Data>::operator=(
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

	template <typename Data>
	InvalidateList<Data>::InvalidateList(InvalidateList&& other) noexcept
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

	template <typename Data>
	InvalidateList<Data>& InvalidateList<Data>::operator=(
		InvalidateList&& other) noexcept
	{
		if (&other == this) return *this;
		delete[] m_pData;
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

	template <typename Data>
	int InvalidateList<Data>::Add(const Data& data)
	{
		const int id{ InternalPreAdd() };
		m_pData[id] = data;
		InternalPostAdd();
		return id;
	}

	template <typename Data>
	int InvalidateList<Data>::Add(Data&& data)
	{
		const int id{ InternalPreAdd() };
		m_pData[id] = std::move(data);
		InternalPostAdd();
		return id;
	}

	template <typename Data>
	int InvalidateList<Data>::Validate(Data*& pOut)
	{
#ifdef INVALIDATE_LIST_DEBUG
		if (!IsEmpty(m_GapIndicator))
			Logger::PrintError("[InvalidateList::Add] GapIndicator is not empty");
#endif
		m_Changed = true;
		const int idx{ m_GapIndicator };
		if (idx >= m_End) m_End = idx + 1;
		else if (idx < m_First) m_First = idx;
		UpdateGapIndicator();
		pOut = &m_pData[idx];
		return idx;
	}

	template <typename Data>
	Data InvalidateList<Data>::Remove(int idx)
	{
		const Data removed{ std::move(m_pData[idx]) };
		Invalidate(idx);
		return removed;
	}

	template <typename Data>
	Data InvalidateList<Data>::InvalidateAndReturn(int idx)
	{
		const Data invalidated{ m_pData[idx] };
		Invalidate(idx);
		return invalidated;
	}

	template <typename Data>
	void InvalidateList<Data>::Invalidate(int idx)
	{
#ifdef INVALIDATE_LIST_DEBUG
		if (idx == m_GapIndicator)
			Logger::PrintError("[InvalidateList::Remove] idx is same as gap-indicator");
		if (idx >= m_End)
			Logger::PrintError("[InvalidateList::Remove] idx is equal or bigger than end");
#endif
		m_Changed = true;
		m_pData[idx].Invalidate();

		if (m_End - m_First == 1)
		{
			m_First = 0;
			m_End = 0;
			m_GapIndicator = 0;
			return;
		}
		if (idx < m_GapIndicator) m_GapIndicator = idx;
		if (idx == m_First) UpdateFirstIndicator();
		else if (idx == m_End - 1) UpdateEndIndicator();
	}

	template <typename Data>
	void InvalidateList<Data>::Clear()
	{
		m_Changed = m_First != 0 || m_End != 0;
		for (unsigned i = m_First; i < m_End; i++)
			m_pData[i].Invalidate();
		m_First = 0;
		m_End = 0;
		m_GapIndicator = 0;
	}

	template <typename Data>
	bool InvalidateList<Data>::HasChanged() const
	{
		return m_Changed;
	}

	template <typename Data>
	void InvalidateList<Data>::ClearChangedFlag()
	{
		m_Changed = false;
	}

	template <typename Data>
	const Data* InvalidateList<Data>::GetData() const
	{
		return m_pData;
	}

	template <typename Data>
	const Data* InvalidateList<Data>::GetFirst() const
	{
		return &m_pData[m_First];
	}

	template <typename Data>
	Data* InvalidateList<Data>::GetFirst()
	{
		m_Changed = true;
		return &m_pData[m_First];
	}

	template <typename Data>
	const Data* InvalidateList<Data>::GetLast() const
	{
		return &m_pData[m_End - 1];
	}

	template <typename Data>
	const Data* InvalidateList<Data>::GetEnd() const
	{
		return &m_pData[m_End];
	}

	template <typename Data>
	Data* InvalidateList<Data>::GetEnd()
	{
		m_Changed = true;
		return &m_pData[m_End];
	}

	template <typename Data>
	Data* InvalidateList<Data>::GetData()
	{
		m_Changed = true;
		return m_pData;
	}

	template <typename Data>
	const Data* InvalidateList<Data>::SeeData() const
	{
		return m_pData;
	}

	template <typename Data>
	const Data* InvalidateList<Data>::SeeFirst() const
	{
		return &m_pData[m_First];
	}

	template <typename Data>
	const Data* InvalidateList<Data>::SeeLast() const
	{
		return &m_pData[m_End - 1];
	}

	template <typename Data>
	const Data* InvalidateList<Data>::SeeEnd() const
	{
		return &m_pData[m_End];
	}

	template <typename Data>
	const Data& InvalidateList<Data>::Get(int idx) const
	{
		return m_pData[idx];
	}

	template <typename Data>
	Data& InvalidateList<Data>::Get(int idx)
	{
		m_Changed = true;
		return m_pData[idx];
	}

	template <typename Data>
	const Data& InvalidateList<Data>::GetSinceStart(unsigned idx) const
	{
		return m_pData[m_First + idx];
	}

	template <typename Data>
	Data& InvalidateList<Data>::GetSinceStart(unsigned idx)
	{
		m_Changed = true;
		return m_pData[m_First + idx];
	}

	template <typename Data>
	Data* InvalidateList<Data>::GetLast()
	{
		m_Changed = true;
		return m_pData[m_End - 1];
	}

	template <typename Data>
	bool InvalidateList<Data>::IsEmpty(int idx) const
	{
		return !m_pData[idx].IsValid();
	}

	template <typename Data>
	void InvalidateList<Data>::UpdateFirstIndicator()
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

	template <typename Data>
	void InvalidateList<Data>::UpdateGapIndicator()
	{
		while (++m_GapIndicator < m_End)
			if (IsEmpty(m_GapIndicator)) return;

		if (static_cast<unsigned>(m_GapIndicator) == m_Capacity)
			IncreaseCapacity(m_Capacity);
	}

	template <typename Data>
	int InvalidateList<Data>::InternalPreAdd()
	{
#ifdef INVALIDATE_LIST_DEBUG
		if (!IsEmpty(m_GapIndicator))
			Logger::PrintError("[InvalidateList::Add] GapIndicator is not empty");
#endif
		m_Changed = true;
		return m_GapIndicator;
	}

	template <typename Data>
	void InvalidateList<Data>::InternalPostAdd()
	{
		if (m_GapIndicator >= m_End) m_End = m_GapIndicator + 1;
		else if (m_GapIndicator < m_First) m_First = m_GapIndicator;
		UpdateGapIndicator();
	}

	template <typename Data>
	void InvalidateList<Data>::UpdateEndIndicator()
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

	template <typename Data>
	void InvalidateList<Data>::IncreaseCapacity(unsigned increase)
	{
		Data* pNew = new Data[m_Capacity + increase];
		std::move(&m_pData[m_First], &m_pData[m_End], &pNew[m_First]);
		delete[] m_pData;
		m_pData = pNew;
		m_Capacity = m_Capacity + increase;
		for (unsigned i = m_Capacity - increase; i < m_Capacity; i++)
			m_pData[i].Invalidate();
	}
}
