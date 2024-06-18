#pragma once

#define SORTED_LIST_DEBUG
#include "Logger/Logger.h"

namespace MyEngine
{
	//has unique items
	template<typename T>
	class SortedList
	{
	public:
		//---| Constructor/Destructor |---
		SortedList();
		SortedList(unsigned capacity);
		~SortedList();

		//---| Move/Copy |---
		SortedList(const SortedList& other);
		SortedList(SortedList&& other) noexcept;
		SortedList& operator=(const SortedList& other);
		SortedList& operator=(SortedList&& other) noexcept;

		//---| Functions |---
		unsigned GetSize() const { return m_Size; }
		const T& operator[](int idx) const;
		const T& First() { return *m_pData; }
		const T& Last() { return m_pData[m_Size - 1]; }

		bool TryAdd(const T& newValue);
		void Clear();
		void RemoveSection(unsigned idx, unsigned sectionLength);
		bool RemoveSection(const T& value, unsigned sectionLength);
		int Find(const T& value) const;
		void Edit(unsigned idx, const T& newValue);

		const T* GetData() const { return m_pData; }

	private:
		unsigned m_Size;
		unsigned m_Capacity;
		T* m_pData;
	};

	template <typename T>
	SortedList<T>::SortedList()
		:m_Size{ 0 }
		, m_Capacity{ 5 }
		, m_pData{ new T[5] }
	{
	}

	template <typename T>
	SortedList<T>::SortedList(unsigned capacity)
		: m_Size{ 0 }
		, m_Capacity{ capacity }
		, m_pData{ new T[capacity] }
	{
	}

	template <typename T>
	SortedList<T>::~SortedList()
	{
		delete[] m_pData;
		m_pData = nullptr;
	}

	template <typename T>
	SortedList<T>::SortedList(const SortedList& other)
		: m_Size{ other.m_Size }
		, m_Capacity{ other.m_Capacity }
		, m_pData{ new T[other.m_Capacity] }
	{
		memcpy(m_pData, other.m_pData, m_Size * sizeof(T));
	}

	template <typename T>
	SortedList<T>::SortedList(SortedList&& other) noexcept
		: m_Size{ other.m_Size }
		, m_Capacity{ other.m_Capacity }
		, m_pData{ other.m_pData }
	{
		other.m_pData = nullptr;
		other.m_Size = 0;
		other.m_Capacity = 0;
	}

	template <typename T>
	SortedList<T>& SortedList<T>::operator=(const SortedList& other)
	{
		if (&other == this) return *this;
		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;
		m_pData = new T[m_Capacity];
		memcpy(m_pData, other.m_pData, m_Size * sizeof(T));
		return *this;
	}

	template <typename T>
	SortedList<T>& SortedList<T>::operator=(SortedList&& other) noexcept
	{
		if (&other == this) return *this;
		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;
		m_pData = other.m_pData;
		other.m_pData = nullptr;
		other.m_Capacity = 0;
		other.m_Size = 0;
		return *this;
	}

	template <typename T>
	const T& SortedList<T>::operator[](int idx) const
	{
		return m_pData[idx];
	}

	template <typename T>
	bool SortedList<T>::TryAdd(const T& newValue)
	{
		for (unsigned i = 0; i < m_Size; i++)
		{
			if (m_pData[i] == newValue) return false;
			if (m_pData[i] < newValue) continue;
			//add
			if (m_Size < m_Capacity)
			{
				memcpy(&m_pData[i + 1], &m_pData[i], (m_Size++ - i) * sizeof(T));
				m_pData[i] = newValue;
			}
			else
			{
				m_Capacity *= 2;
				T* pNew = new T[m_Capacity];
				memcpy(pNew, &m_pData[0], i * sizeof(T));
				pNew[i] = newValue;
				memcpy(&pNew[i + 1], &m_pData[i], (m_Size++ - i) * sizeof(T));
				delete[] m_pData;
				m_pData = pNew;
			}
			return true;
		}
		//add at end
		if (m_Size < m_Capacity)
			m_pData[m_Size++] = newValue;
		else
		{
			m_Capacity *= 2;
			T* pNew = new T[m_Capacity];
			pNew[m_Size] = newValue;
			memcpy(pNew, m_pData, m_Size++ * sizeof(T));
			delete[] m_pData;
			m_pData = pNew;
		}
		return true;
	}

	template <typename T>
	void SortedList<T>::Clear()
	{
		m_Size = 0;
	}

	template <typename T>
	void SortedList<T>::RemoveSection(unsigned idx, unsigned sectionLength)
	{
		std::move(&m_pData[idx + sectionLength], &m_pData[m_Size], &m_pData[idx]);
		m_Size -= sectionLength;
	}

	template <typename T>
	bool SortedList<T>::RemoveSection(const T& value, unsigned sectionLength)
	{
		const int i = Find(value);
		if (i == -1)
		{
#ifdef SORTED_LIST_DEBUG
			Logger::PrintError("[SortedList::RemoveSection] Value not found");
#endif
			return false;
		}
		RemoveSection(static_cast<unsigned>(i), sectionLength);
		return true;
	}

	template <typename T>
	int SortedList<T>::Find(const T& value) const
	{
		for (int i = 0; i < static_cast<int>(m_Size); i++)
			if (value == m_pData[i])
				return i;
		return -1;
	}

	template <typename T>
	void SortedList<T>::Edit(unsigned idx, const T& newValue)
	{
		bool previousOk{ true };
		bool nextOk{ true };

		if (idx > 0)
			previousOk = m_pData[idx - 1] < newValue;
		if (idx + 1 < m_Size)
			nextOk = m_pData[idx + 1] > newValue;

		if (previousOk && nextOk)
		{
			m_pData[idx] = newValue;
			return;
		}

		RemoveSection(idx, 1);
		TryAdd(newValue);
	}
}
