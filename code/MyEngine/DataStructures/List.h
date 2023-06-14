#pragma once

namespace MyEngine
{
	namespace Ds
	{
		template<typename T>
		class List
		{
		public:
			//---| Constructor/Destructor |---
			List(int capacity = 5);
			~List();

			//---| Move/Copy |---
			List(const List& other);
			List(List&& other) noexcept;
			List& operator=(const List& other);
			List& operator=(List&& other) noexcept;

			//---| Functions |---
			void Add(const T& value);
			void Add(T&& value);
			void Insert(int idx, const T& value);
			void InsertEmpty(int idx, int amount);

			const T& operator[](int idx) const;
			T& operator[](int idx);
			void Clear();

			const T* GetData() const { return m_pData; }
			int GetSize() const { return m_Size; }
			unsigned GetSizeU() const { return static_cast<unsigned>(m_Size); }
			int GetCapacity() const { return m_Capacity; }
			unsigned GetCapacityU() const { return static_cast<unsigned>(m_Capacity); }
			bool IsEmpty() const { return m_Size == 0; }

		private:
			T* m_pData;
			int m_Capacity;
			int m_Size;

			void IncreaseCapacity();
		};

		template <typename T>
		List<T>::List(int capacity)
			: m_pData{ new T[capacity] }
			, m_Capacity{ capacity }
			, m_Size{ 0 }
		{ }

		template <typename T>
		List<T>::~List()
		{
			delete[] m_pData;
		}

		template <typename T>
		List<T>::List(const List& other)
			: m_pData{ new T[other.m_Capacity] }
			, m_Capacity{ other.m_Capacity }
			, m_Size{ other.m_Size }
		{
			memcpy(m_pData, other.m_pData, sizeof(T) * m_Size);
		}

		template <typename T>
		List<T>::List(List&& other) noexcept
			: m_pData{ other.m_pData }
			, m_Capacity{ other.m_Capacity }
			, m_Size{ other.m_Size }
		{
			other.m_pData = nullptr;
		}

		template <typename T>
		List<T>& List<T>::operator=(const List& other)
		{
			if (&other == this) return *this;
			delete[] m_pData;
			m_pData = new T[other.m_Size];
			memcpy(m_pData, other.m_pData, sizeof(T) * other.m_Size);
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;
			return *this;
		}

		template <typename T>
		List<T>& List<T>::operator=(List&& other) noexcept
		{
			if (&other == this) return *this;
			delete[] m_pData;
			m_pData = other.m_pData;
			other.m_pData = nullptr;
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;
			return *this;
		}

		template <typename T>
		void List<T>::Add(const T& value)
		{
			if (m_Size == m_Capacity)
				IncreaseCapacity();
			m_pData[m_Size++] = value;
		}

		template <typename T>
		void List<T>::Add(T&& value)
		{
			if (m_Size == m_Capacity)
				IncreaseCapacity();
			m_pData[m_Size++] = std::move(value);
		}

		template <typename T>
		void List<T>::Insert(int idx, const T& value)
		{
			if (m_Size == m_Capacity)
			{
				m_Capacity *= 2;
				T* pNew = new T[m_Capacity];
				memcpy(pNew, m_pData, idx * sizeof(T));
				pNew[idx] = value;
				memcpy(&pNew[idx + 1], &m_pData[idx], (m_Size - idx) * sizeof(T));
				delete m_pData;
				m_pData = pNew;
				m_Size++;
				return;
			}
			memcpy(&m_pData[idx], &m_pData[idx + 1], (m_Size - idx) * sizeof(T));
			m_pData[idx] = value;
			m_Size++;
		}

		template <typename T>
		void List<T>::InsertEmpty(int idx, int amount)
		{
			const int newSize = m_Size + amount;
			if (newSize <= m_Capacity)
			{
				memcpy(&m_pData[idx + amount], &m_pData[idx], (m_Size - idx) * sizeof(T));
				m_Size = newSize;
				return;
			}
			m_Capacity = newSize * 2;
			T* pNew = new T[m_Capacity];
			memcpy(pNew, m_pData, idx * sizeof(T));
			memcpy(&pNew[idx + amount], &m_pData[idx], (m_Size - idx) * sizeof(T));
			delete m_pData;
			m_pData = pNew;
			m_Size = newSize;
		}

		template <typename T>
		const T& List<T>::operator[](int idx) const
		{
			return m_pData[idx];
		}

		template <typename T>
		T& List<T>::operator[](int idx)
		{
			return m_pData[idx];
		}

		template <typename T>
		void List<T>::Clear()
		{
			m_Size = 0;
		}

		template <typename T>
		void List<T>::IncreaseCapacity()
		{
			m_Capacity *= 2;
			T* pNew = new T[m_Capacity];
			memcpy(pNew, m_pData, sizeof(T) * m_Size);
			delete m_pData;
			m_pData = pNew;
		}
	}
}