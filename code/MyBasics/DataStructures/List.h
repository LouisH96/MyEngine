#pragma once

namespace MyEngine
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
		void Add(const T* pValue, unsigned nrValues);
		void Add(const T& value1, const T& value2);
		void Add(const T& value1, const T& value2, const T& value3);
		void Insert(int idx, const T& value);
		void InsertEmpty(int idx, int amount);

		const T& operator[](int idx) const;
		T& operator[](int idx);
		void Clear();

		const T* GetData() const { return m_pData; }
		const T& First() const { return m_pData[0]; }
		const T& Last() const { return m_pData[m_Size - 1]; }
		T& First() { return m_pData[0]; }
		T& Last() { return m_pData[m_Size - 1]; }
		int GetSize() const { return m_Size; }
		unsigned GetSizeU() const { return static_cast<unsigned>(m_Size); }
		int GetCapacity() const { return m_Capacity; }
		unsigned GetCapacityU() const { return static_cast<unsigned>(m_Capacity); }
		bool IsEmpty() const { return m_Size == 0; }
		void SetSize(int size);
		void EnsureCapacity(unsigned minCapacity);
		void EnsureIncrease(unsigned increase);
		void DeleteAll();

	private:
		T* m_pData;
		int m_Capacity;
		int m_Size;

		void SetCapacity(int capacity);
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
		m_pData = nullptr;
		m_Capacity = 0;
		m_Size = 0;
	}

	template <typename T>
	List<T>::List(const List& other)
		: m_pData{ new T[other.m_Capacity] }
		, m_Capacity{ other.m_Capacity }
		, m_Size{ other.m_Size }
	{
		std::copy(&other.m_pData[0], &other.m_pData[other.m_Size], &m_pData[0]);
	}

	template <typename T>
	List<T>::List(List&& other) noexcept
		: m_pData{ other.m_pData }
		, m_Capacity{ other.m_Capacity }
		, m_Size{ other.m_Size }
	{
		other.m_pData = nullptr;
		other.m_Capacity = 0;
		other.m_Size = 0;
	}

	template <typename T>
	List<T>& List<T>::operator=(const List& other)
	{
		if (&other == this) return *this;
		delete[] m_pData;
		m_pData = new T[other.m_Capacity];
		std::copy(&other.m_pData[0], &other.m_pData[other.m_Size], &m_pData[0]);
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
		m_Capacity = other.m_Capacity;
		m_Size = other.m_Size;
		other.m_pData = nullptr;
		other.m_Capacity = 0;
		other.m_Size = 0;
		return *this;
	}

	template <typename T>
	void List<T>::Add(const T& value)
	{
		if (m_Size == m_Capacity)
			SetCapacity(m_Capacity * 2 + 1);
		m_pData[m_Size++] = value;
	}

	template <typename T>
	void List<T>::Add(T&& value)
	{
		if (m_Size == m_Capacity)
			SetCapacity(m_Capacity * 2 + 1);
		m_pData[m_Size++] = std::move(value);
	}

	template <typename T>
	void List<T>::Add(const T* pValue, unsigned nrValues)
	{
		const unsigned newSize{ m_Size + nrValues };
		if (newSize > m_Capacity) SetCapacity(newSize * 2);
		std::copy(&pValue[0], &pValue[nrValues], &m_pData[m_Size]);
		m_Size = newSize;
	}

	template <typename T>
	void List<T>::Add(const T& value1, const T& value2)
	{
		EnsureIncrease(2);
		m_pData[m_Size++] = value1;
		m_pData[m_Size++] = value2;
	}

	template <typename T>
	void List<T>::Add(const T& value1, const T& value2, const T& value3)
	{
		EnsureIncrease(3);
		m_pData[m_Size++] = value1;
		m_pData[m_Size++] = value2;
		m_pData[m_Size++] = value3;
	}

	template <typename T>
	void List<T>::Insert(int idx, const T& value)
	{
		if (m_Size == m_Capacity)
		{
			m_Capacity *= 2;
			T* pNew = new T[m_Capacity];
			std::move(&m_pData[0], &m_pData[idx], &pNew[0]);
			pNew[idx] = value;
			std::move(&m_pData[idx], &m_pData[m_Size], &pNew[idx + 1]);
			delete[] m_pData;
			m_pData = pNew;
			m_Size++;
			return;
		}
		std::move_backward(&m_pData[idx], &m_pData[m_Size], &m_pData[m_Size + 1]);
		m_pData[idx] = value;
		m_Size++;
	}

	template <typename T>
	void List<T>::InsertEmpty(int idx, int amount)
	{
		const int newSize = m_Size + amount;
		if (newSize <= m_Capacity)
		{
			std::move(&m_pData[idx], &m_pData[m_Size], &m_pData[idx + 1]);
			m_Size = newSize;
			return;
		}
		m_Capacity = newSize * 2;
		T* pNew = new T[m_Capacity];
		std::move(&m_pData[0], &m_pData[idx], &pNew[0]);
		std::move(&m_pData[idx], &m_pData[m_Size], &pNew[idx + amount]);
		delete[] m_pData;
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
		for (int i = 0; i < m_Size; i++)
			m_pData[i].~T();
		m_Size = 0;
	}

	template <typename T>
	void List<T>::SetSize(int size)
	{
		if (size > m_Capacity) SetCapacity(size);
		m_Size = size;
	}

	template <typename T>
	void List<T>::EnsureCapacity(unsigned minCapacity)
	{
		if (m_Capacity >= minCapacity) return;
		SetCapacity(minCapacity);
	}

	template <typename T>
	void List<T>::EnsureIncrease(unsigned increase)
	{
		EnsureCapacity(m_Capacity + increase);
	}

	template <typename T>
	void List<T>::DeleteAll()
	{
		for (unsigned i = 0; i < m_Size; i++)
			delete m_pData[i];
	}

	//should be bigger than current size
	template <typename T>
	void List<T>::SetCapacity(int capacity)
	{
		m_Capacity = capacity;
		T* pNew = new T[m_Capacity];
		std::move(&m_pData[0], &m_pData[m_Size], &pNew[0]);
		delete[] m_pData;
		m_pData = pNew;
	}
}
