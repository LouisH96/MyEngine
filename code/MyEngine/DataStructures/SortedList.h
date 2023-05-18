#pragma once

namespace MyEngine
{
	namespace Ds
	{
		//has unique items
		template<typename T>
		class SortedList
		{
		public:
			//---| Constructor/Destructor |---
			SortedList();
			SortedList(int capacity);
			~SortedList();

			//---| Move/Copy |---
			SortedList(const SortedList& other);
			SortedList(SortedList&& other) noexcept;
			SortedList& operator=(const SortedList& other);
			SortedList& operator=(SortedList&& other) noexcept;

			//---| Functions |---
			int GetSize() const { return m_Size; }
			const T& operator[](int idx) const;

			bool TryAdd(const T& newValue);


		private:
			int m_Size;
			int m_Capacity;
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
		SortedList<T>::SortedList(int capacity)
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
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;
			m_pData = new T[m_Capacity];
			memcpy(m_pData, other.m_pData, m_Size * sizeof(T));
			return *this;
		}

		template <typename T>
		SortedList<T>& SortedList<T>::operator=(SortedList&& other) noexcept
		{
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
			for (int i = 0; i < m_Size; i++)
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
					memcpy(&pNew, &m_pData[0], i * sizeof(T));
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
				memcpy(pNew, m_pData, m_Size++ * sizeof(T));
				delete[] m_pData;
				m_pData = pNew;
			}
			return true;
		}
	}
}