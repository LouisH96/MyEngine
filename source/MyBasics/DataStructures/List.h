#pragma once

#include "Array.h"
//#define LIST_DEBUG

namespace MyEngine
{
template<typename T>
class List
{
public:
	using DataType = T;

	//---| Constructor/Destructor |---
	List(unsigned capacity = 5);
	List(int capacity);
	~List();

	List(Array<T>&& array);

	//---| Move/Copy |---
	List(const List& other);
	List(List&& other) noexcept;
	List& operator=(const List& other);
	List& operator=(List&& other) noexcept;

	//---| Functions |---
	T& Add();
	void Add(const T& value);
	void Add(T&& value);
	void Add(const T* pValue, unsigned nrValues);
	void Add(const T& value1, const T& value2);
	void Add(const T& value1, const T& value2, const T& value3);
	void Add(const List& list);
	void Add(const Array<T>& arr);
	void AddMultipleTimes(const T& value, unsigned count);
	void Insert(int idx, const T& value);
	void InsertEmpty(int idx, int amount);

	void Remove(unsigned idx);

	const T& operator[](int idx) const;
	const T& operator[](unsigned idx) const;
	T& operator[](int idx);
	T& operator[](unsigned idx);

	void Clear();

	const T* GetData() const { return m_pData; }
	const T& First() const { return m_pData[0]; }
	const T& Last() const { return m_pData[m_Size - 1]; }
	const T* GetEnd() const { return &m_pData[m_Size]; }
	T& First() { return m_pData[0]; }
	T& Last() { return m_pData[m_Size - 1]; }
	int GetSizeS() const { return static_cast<int>(m_Size); }
	unsigned GetSize() const { return m_Size; }
	int GetCapacityS() const { return static_cast<int>(m_Capacity); }
	unsigned GetCapacity() const { return m_Capacity; }
	bool IsEmpty() const { return m_Size == 0; }
	void SetSize(int size);
	void EnsureCapacity(unsigned minCapacity);
	void EnsureIncrease(unsigned increase);
	void DeleteAll();
	void IncreaseSize(unsigned amount);
	void ReduceSize(unsigned amount);

	bool Any() const { return m_Size > 0; }
	bool Empty() const { return m_Size == 0; }

	Array<T> ToArray();

private:
	T* m_pData;
	unsigned m_Capacity;
	unsigned m_Size;

	void SetCapacity(int capacity);

	//---| AddHelper |---
	class _AddHelper
	{
	public:
		_AddHelper(unsigned amount, List& list);
		~_AddHelper();

	private:
		T* m_pOld;
	};
	_AddHelper _PreAdd(unsigned amount);

	friend class _AddHelper;
};

template <typename T>
List<T>::List(int capacity)
	: m_pData{ new T[capacity] }
	, m_Capacity{ static_cast<unsigned>(capacity) }
	, m_Size{ 0 }
{
}

template <typename T>
List<T>::List(unsigned capacity)
	: m_pData{ new T[capacity] }
	, m_Capacity{ capacity }
	, m_Size{ 0 }
{

}

template <typename T>
List<T>::~List()
{
	delete[] m_pData;
	m_pData = nullptr;
	m_Capacity = 0;
	m_Size = 0;
}

template<typename T>
inline List<T>::List(Array<T>&& array)
	: m_pData{}
	, m_Capacity{ array.GetSize() }
	, m_Size{ array.GetSize() }
{
	m_pData = array.TakeData();
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
T& List<T>::Add()
{
	if (m_Size == m_Capacity)
		SetCapacity(m_Capacity * 2 + 1);
	return m_pData[m_Size++];
}

template <typename T>
void List<T>::Add(const T& value)
{
	_AddHelper helper{ _PreAdd(1) };
	m_pData[m_Size++] = value;
}

template <typename T>
void List<T>::Add(T&& value)
{
	_AddHelper helper{ _PreAdd(1) };
	m_pData[m_Size++] = std::move(value);
}

template <typename T>
void List<T>::Add(const T* pValue, unsigned nrValues)
{
	_AddHelper helper{ _PreAdd(nrValues) };

	std::copy(&pValue[0], &pValue[nrValues], &m_pData[m_Size]);
	m_Size += nrValues;
}

template <typename T>
void List<T>::Add(const T& value1, const T& value2)
{
	_AddHelper helper{ _PreAdd(2) };

	m_pData[m_Size++] = value1;
	m_pData[m_Size++] = value2;
}

template <typename T>
void List<T>::Add(const T& value1, const T& value2, const T& value3)
{
	_AddHelper helper{ _PreAdd(3) };

	m_pData[m_Size++] = value1;
	m_pData[m_Size++] = value2;
	m_pData[m_Size++] = value3;
}

template <typename T>
void List<T>::Add(const List& list)
{
	_AddHelper helper{ _PreAdd(list.GetSize()) };

	std::copy(list.GetData(), &list.GetData()[list.GetSize()], &m_pData[m_Size]);
	m_Size += list.GetSize();
}

template <typename T>
void List<T>::Add(const Array<T>& arr)
{
	_AddHelper helper{ _PreAdd(arr.GetSize()) };

	std::copy(arr.GetData(), &arr.GetData()[arr.GetSize()], &m_pData[m_Size]);
	m_Size += arr.GetSize();
}

template <typename T>
void List<T>::AddMultipleTimes(const T& value, unsigned count)
{
	_AddHelper helper{ _PreAdd(count) };

	for (unsigned i = 0; i < count; i++)
		m_pData[m_Size++] = value;
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
void List<T>::Remove(unsigned idx)
{
	std::move(&m_pData[idx + 1], &m_pData[m_Size--], &m_pData[idx]);
}

template <typename T>
const T& List<T>::operator[](int idx) const
{
	return m_pData[idx];
}

template <typename T>
const T& List<T>::operator[](unsigned idx) const
{
	return m_pData[idx];
}

template <typename T>
T& List<T>::operator[](int idx)
{
	return m_pData[idx];
}

template <typename T>
T& List<T>::operator[](unsigned idx)
{
	return m_pData[idx];
}

template <typename T>
void List<T>::Clear()
{
	for (unsigned i = 0; i < m_Size; i++)
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
	const unsigned required{ m_Size + increase };
	if (required > m_Capacity)
		SetCapacity(required + required);
}

template <typename T>
void List<T>::DeleteAll()
{
	for (unsigned i = 0; i < m_Size; i++)
		delete m_pData[i];
}

template <typename T>
void List<T>::IncreaseSize(unsigned amount)
{
	const unsigned newSize{ m_Size + amount };
	if (newSize > m_Capacity)
		SetCapacity(newSize + newSize);
	m_Size = newSize;
}

template <typename T>
void List<T>::ReduceSize(unsigned amount)
{
#ifdef LIST_DEBUG
	if (amount > m_Size)
		Logger::PrintError("[List::ReduceSize] amount bigger than size");
#endif
	m_Size -= amount;
}

template <typename T>
Array<T> List<T>::ToArray()
{
	Array<T> array{ m_Size };
	std::move(m_pData, &m_pData[m_Size], array.GetData());
	m_Size = 0;
	return array;
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

template<typename T>
inline List<T>::_AddHelper::_AddHelper(unsigned amount, List& list)
{
	if (list.m_Size + amount > list.m_Capacity)
	{
		m_pOld = list.m_pData;
		list.m_pData = new T[(list.m_Size + amount) * 2];
		std::move(m_pOld, &m_pOld[list.m_Size], list.m_pData);
		list.m_Capacity = (list.m_Size + amount) * 2;
	}
	else
		m_pOld = nullptr;
}
template<typename T>
inline List<T>::_AddHelper::~_AddHelper()
{
	delete[] m_pOld;
}

template<typename T>
inline typename List<T>::_AddHelper List<T>::_PreAdd(unsigned amount)
{
	return List<T>::_AddHelper{ amount, *this };
}

}