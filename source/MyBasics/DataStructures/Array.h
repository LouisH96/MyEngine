#pragma once
#include <iostream>
//#define ARRAY_DEBUG

namespace MyEngine
{
	template<typename Data>
	class Array
	{
	public:
		//---| Constructor/Destructor |---
		static Array CopyFrom(const Data* pData, int amount);
		Array();
		Array(int size);
		Array(size_t size);
		Array(unsigned size);
		Array(int size, const Data& initValue);
		Array(unsigned size, const Data& initValue);
		~Array();

		//---| Move/Copy |---
		Array(const Array& other);
		Array(Array&& other) noexcept;
		Array& operator=(const Array& other);
		Array& operator=(Array&& other) noexcept;

		//---| Functions: Add |---
		void Add(const Array& other);
		void Add(const Data& data);
		void Add(const Data&& data);
		void IncreaseSizeTo(int newSize);
		void IncreaseSizeWith(int additionalSize);
		void EnsureSize(unsigned minSize);

		//---| Functions: Remove |---
		void Remove(const Data& value);
		void RemoveAt(int idx);

		//---| Functions: Get Element |---
		Data& operator[](int idx);
		Data& operator[](unsigned idx);
		Data& Last();
		Data& First();
		Data* GetData() { return m_pData; }
		const Data& operator[](int idx) const;
		const Data& operator[](unsigned idx) const;
		const Data& Last() const;
		const Data& First() const;
		const Data* GetData() const { return m_pData; }

		//---| Functions: Insert |---
		void InsertRevert(unsigned idx, const Data& value); //pull back to make space

		//---| Functions: Get Other |---
		unsigned GetSize() const { return m_Size; }
		int GetSizeS() const { return static_cast<int>(m_Size); }
		bool IsEmpty() const { return m_Size == 0; }
		bool Any() const { return m_Size > 0; }

		//---| Functions: Search |---
		template<typename F> Data* First(const F&& func);
		template<typename F> const Data* First(const F&& func) const;
		template<typename F> bool Any(const F&& func) const;
		template<typename F> bool None(const F&& func) const;

		//---| Functions: Sort |---
		void BubbleSort();
		bool IsSorted() const;

		//---| Functions: Other |---
		void DeleteAll() const; //calls delete on all (doesn't clear array)
		void CopyTo(Array& dest, int destIdx);

	private:
		Data* m_pData;
		unsigned m_Size;

		void DoBoundsCheck(unsigned idx) const;
		void PrintOutOfBounds(unsigned idx) const;
	};

	template <typename Data>
	Array<Data> Array<Data>::CopyFrom(const Data* pData, int amount)
	{
		Array array{ amount };
		memcpy(array.m_pData, pData, amount * sizeof(Data));
		return array;
	}

	template <typename Data>
	Array<Data>::Array()
		: m_pData{ nullptr }
		, m_Size{ 0 }
	{
	}

	template <typename Data>
	Array<Data>::Array(int size)
		: m_pData(new Data[size])
		, m_Size(size)
	{
	}

	template <typename Data>
	Array<Data>::Array(Array&& other) noexcept
		: m_pData{ other.m_pData }
		, m_Size{ other.m_Size }
	{
		other.m_pData = nullptr;
		other.m_Size = 0;
	}

	template <typename Data>
	Array<Data>& Array<Data>::operator=(const Array& other)
	{
		if (&other == this) return *this;
		delete[] m_pData;
		m_pData = new Data[other.m_Size];
		m_Size = other.m_Size;
		std::copy(other.m_pData, &other.m_pData[other.m_Size], m_pData);
		return *this;
	}

	template <typename Data>
	Array<Data>& Array<Data>::operator=(Array&& other) noexcept
	{
		if (&other == this) return *this;
		delete[] m_pData;
		m_pData = other.m_pData;
		m_Size = other.m_Size;
		other.m_pData = nullptr;
		other.m_Size = 0;
		return *this;
	}

	template <typename Data>
	Array<Data>::Array(int size, const Data& initValue)
		: m_pData(new Data[size])
		, m_Size(size)
	{
		for (unsigned i = 0; i < m_Size; i++)
			m_pData[i] = initValue;
	}

	template <typename Data>
	Array<Data>::Array(unsigned size, const Data& initValue)
		: m_pData(new Data[size])
		, m_Size(size)
	{
		for (unsigned i = 0; i < m_Size; i++)
			m_pData[i] = initValue;
	}

	template<typename Data>
	Array<Data>::Array(unsigned size)
		: m_pData(new Data[size])
		, m_Size(size)
	{
	}

	template <typename Data>
	Array<Data>::Array(size_t size)
		: m_pData(new Data[size])
		, m_Size{ static_cast<unsigned>(size) }
	{

	}

	template <typename Data>
	Array<Data>::~Array()
	{
		delete[] m_pData;
	}

	template <typename Data>
	Array<Data>::Array(const Array& other)
		: m_pData(new Data[other.m_Size])
		, m_Size(other.m_Size)
	{
		std::copy(other.m_pData, &other.m_pData[m_Size], m_pData);
	}

	template <typename Data>
	void Array<Data>::Add(const Array& other)
	{
		Data* pNew = new Data[m_Size + other.m_Size];
		std::move(m_pData, &m_pData[m_Size], pNew);
		std::copy(other.m_pData, &other.m_pData[other.m_Size], &pNew[m_Size]);
		delete[] m_pData;
		m_pData = pNew;
		m_Size += other.m_Size;
	}

	template <typename Data>
	void Array<Data>::Add(const Data& data)
	{
		Data* pNew = new Data[m_Size + 1];
		std::move(m_pData, &m_pData[m_Size], pNew);
		pNew[m_Size] = data;
		delete[] m_pData;
		m_pData = pNew;
		m_Size++;
	}

	template <typename Data>
	void Array<Data>::Add(const Data&& data)
	{
		Data* pNew = new Data[m_Size + 1];
		std::move(m_pData, &m_pData[m_Size], pNew);
		pNew[m_Size] = std::move(data);
		delete[] m_pData;
		m_pData = pNew;
		m_Size++;
	}

	template <typename Data>
	void Array<Data>::IncreaseSizeTo(int newSize)
	{
		Data* pNew = new Data[newSize];
		std::move(m_pData, &m_pData[m_Size], pNew);
		delete[] m_pData;
		m_pData = pNew;
		m_Size = newSize;
	}

	template <typename Data>
	void Array<Data>::IncreaseSizeWith(int additionalSize)
	{
		IncreaseSizeTo(m_Size + additionalSize);
	}

	template <typename Data>
	void Array<Data>::EnsureSize(unsigned minSize)
	{
		if (minSize > m_Size)
			IncreaseSizeTo(minSize);
	}

	/**
	 * \brief Removes first occurrence.
	 * If not found: last item will be removed
	 */
	template <typename Data>
	void Array<Data>::Remove(const Data& value)
	{
		Data* pNew = new Data[--m_Size];
		for (int i = 0; i < m_Size; i++)
		{
			if (m_pData[i] == value)
			{
				std::move(&m_pData[i + 1], &m_pData[m_Size], &m_pData[i]);
				break;
			}
			pNew[i] = std::move(m_pData[i]);
		}
		delete[] m_pData;
		m_pData = pNew;
	}

	template <typename Data>
	void Array<Data>::RemoveAt(int idx)
	{
		Data* pNew = new Data[--m_Size];
		std::move(m_pData, &m_pData[idx], pNew);
		std::move(&m_pData[idx + 1], &m_pData[m_Size], &pNew[idx]);
		delete[] m_pData;
		m_pData = pNew;
	}

	template <typename Data>
	const Data& Array<Data>::operator[](int idx) const
	{
#ifdef ARRAY_DEBUG
		DoBoundsCheck(idx);
#endif
		return m_pData[idx];
	}

	template <typename Data>
	const Data& Array<Data>::operator[](unsigned idx) const
	{
#ifdef ARRAY_DEBUG
		DoBoundsCheck(idx);
#endif
		return m_pData[idx];
	}

	template <typename Data>
	Data& Array<Data>::operator[](int idx)
	{
#ifdef ARRAY_DEBUG
		DoBoundsCheck(idx);
#endif
		return m_pData[idx];
	}

	template <typename Data>
	Data& Array<Data>::operator[](unsigned idx)
	{
#ifdef ARRAY_DEBUG
		DoBoundsCheck(idx);
#endif
		return m_pData[idx];
	}

	template <typename Data>
	Data& Array<Data>::Last()
	{
		return m_pData[m_Size - 1];
	}

	template <typename Data>
	Data& Array<Data>::First()
	{
		return m_pData[0];
	}

	template <typename Data>
	const Data& Array<Data>::Last() const
	{
		return m_pData[m_Size - 1];
	}

	template <typename Data>
	const Data& Array<Data>::First() const
	{
		return m_pData[0];
	}

	template <typename Data>
	void Array<Data>::InsertRevert(unsigned idx, const Data& value)
	{
		std::move(&m_pData[1], &m_pData[idx + 1], &m_pData[0]);
		m_pData[idx] = value;
	}

	template <typename Data>
	void Array<Data>::DeleteAll() const
	{
		for (unsigned i = 0; i < m_Size; i++)
			delete m_pData[i];
	}

	template <typename Data>
	void Array<Data>::CopyTo(Array& dest, int destIdx)
	{
		std::copy(m_pData, &m_pData[m_Size], &dest.m_pData[destIdx]);
	}

	template <typename Data>
	template <typename F>
	bool Array<Data>::Any(const F&& func) const
	{
		for (int i = 0; i < m_Size; i++)
			if (func(m_pData[i]))
				return true;
		return false;
	}

	template <typename Data>
	template <typename F>
	bool Array<Data>::None(const F&& func) const
	{
		for (int i = 0; i < m_Size; i++)
			if (func(m_pData[i]))
				return false;
		return true;
	}

	template <typename Data>
	void Array<Data>::BubbleSort()
	{
		unsigned lastToCheck{ m_Size - 1 };
		bool changed{ true };
		while (lastToCheck > 0 && changed)
		{
			changed = false;
			for (unsigned i = 0; i < lastToCheck; i++)
			{
				if (m_pData[i] > m_pData[i + 1])
				{
					const Data temp{ m_pData[i + 1] };
					m_pData[i + 1] = m_pData[i];
					m_pData[i] = temp;
					changed = true;
				}
			}
			lastToCheck--;
		}
	}

	/**
	 * \brief ascending
	 */
	template <typename Data>
	bool Array<Data>::IsSorted() const
	{
		for (unsigned i = 0; i + 1 < m_Size; i++)
			if (m_pData[i] > m_pData[i + 1])
				return false;
		return true;
	}

	template<typename Data>
	template<typename F>
	Data* Array<Data>::First(const F&& func)
	{
		for (int i = 0; i < m_Size; i++)
			if (func(m_pData[i]))
				return m_pData;
		return nullptr;
	}

	template<typename Data>
	template<typename F>
	const Data* Array<Data>::First(const F&& func) const
	{
		for (int i = 0; i < m_Size; i++)
			if (func(m_pData[i]))
				return m_pData;
		return nullptr;
	}

	template <typename Data>
	void Array<Data>::DoBoundsCheck(unsigned idx) const
	{
		if (idx < 0 || idx >= m_Size)
			PrintOutOfBounds(idx);
	}

	template <typename Data>
	void Array<Data>::PrintOutOfBounds(unsigned idx) const
	{
		std::cout << "ArrayIdx " << idx
			<< " out of bounds[0-" << m_Size - 1 << "]\n";
	}
}
