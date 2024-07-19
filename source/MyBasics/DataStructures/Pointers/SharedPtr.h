#pragma once

namespace MyEngine
{
struct RefCounter
{
	RefCounter() = default;
	RefCounter(unsigned count)
		: Count{ count }
	{};

	unsigned Count{ 0 };
};

template<typename T>
struct SharedPtrContent
	: public RefCounter
{
	SharedPtrContent() = default;
	SharedPtrContent(unsigned count, const T& value)
		: RefCounter{ count }
		, Value{ value }
	{};

	T Value;
};

template<typename T>
class SharedPtr
{
private:
	SharedPtr(RefCounter* pCounter, T* pValue);

	void CopyConstructor();
	template<typename O>
	void MoveConstructor(SharedPtr<O>& other);
	template<typename O>
	SharedPtr& CopyOperator(const SharedPtr<O>& other);
	template<typename O>
	SharedPtr& MoveOperator(SharedPtr<O>&& other);

public:
	SharedPtr() = default;

	template<typename O>
	SharedPtr(const O& derived);
	SharedPtr(const T& value);
	~SharedPtr();

	template<typename O>
	SharedPtr(const SharedPtr<O>& other);
	SharedPtr(const SharedPtr& other);

	template<typename O>
	SharedPtr(SharedPtr<O>&& other);
	SharedPtr(SharedPtr&& other);

	template<typename O>
	SharedPtr<T>& operator=(const SharedPtr<O>& other);
	SharedPtr<T>& operator=(const SharedPtr<T>& other);

	template<typename O>
	SharedPtr<T>& operator=(SharedPtr<O>&& other);
	SharedPtr<T>& operator=(SharedPtr<T>&& other);

	const T& Get() const;
	T& Get();

private:
	RefCounter* m_pCounter{};
	T* m_pValue{};

	template<typename O>
	friend class SharedPtr;
};

template<typename T>
inline SharedPtr<T>::SharedPtr(RefCounter* pCounter, T* pValue)
	: m_pCounter{ pCounter }
	, m_pValue{ pValue }
{
}

template<typename T>
inline void SharedPtr<T>::CopyConstructor()
{
	if (m_pCounter)
		m_pCounter->Count++;
}

template<typename T>
template<typename O>
inline void SharedPtr<T>::MoveConstructor(SharedPtr<O>& other)
{
	other.m_pCounter = nullptr;
}

template<typename T>
template<typename O>
inline SharedPtr<T>& SharedPtr<T>::CopyOperator(const SharedPtr<O>& other)
{
	if (&other == this)
		return;

	if (other.m_pCounter == m_pCounter)
		return;

	if (m_pCounter)
	{
		if (m_pCounter->Count == 1)
			delete m_pCounter;
		else
			m_pCounter->Count--;
	}

	m_pCounter = other.m_pCounter;
	if (m_pCounter)
	{
		m_pCounter.count++;
		m_pValue = other.m_pValue;
	}

	return *this;
}

template<typename T>
template<typename O>
inline SharedPtr<T>& SharedPtr<T>::MoveOperator(SharedPtr<O>&& other)
{
	if (&other == this) return;

	if (other.m_pCounter == m_pCounter)
	{
		if (m_pCounter)
		{
			m_pCounter.count--;
			other.m_pCounter = nullptr;
		}
		return;
	}

	if (m_pCounter)
	{
		if (m_pCounter->Count == 1)
			delete m_pCounter;
		else
			m_pCounter->Count--;
	}

	m_pCounter = other.m_pCounter;
	other.m_pCounter = nullptr;

	if (m_pCounter)
		m_pValue = other.m_pValue;

	return *this;
}

template<typename T>
template<typename O>
inline SharedPtr<T>::SharedPtr(const O& derived)
{
	SharedPtrContent<O>* pContent{ new SharedPtrContent<O>(1, derived) };
	m_pCounter = pContent;
	m_pValue = &pContent->Value;
}
template<typename T>
inline SharedPtr<T>::SharedPtr(const T& value)
{
	SharedPtrContent<T>* pContent{ new SharedPtrContent<T>(1, value) };
	m_pCounter = pContent;
	m_pValue = &pContent->Value;
}
template<typename T>
inline SharedPtr<T>::~SharedPtr()
{
	if (!m_pCounter)
		return;
	if (m_pCounter->Count == 1)
	{
		delete m_pCounter;
		m_pCounter = nullptr;
	}
	else
		m_pCounter->Count--;
}
template<typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
	: SharedPtr{ other.m_pCounter, other.m_pValue }
{
	CopyConstructor();
}
template<typename T>
template<typename O>
inline SharedPtr<T>::SharedPtr(const SharedPtr<O>& other)
	: SharedPtr{ other.m_pCounter, other.m_pValue }
{
	CopyConstructor();
}
template<typename T>
template<typename O>
inline SharedPtr<T>::SharedPtr(SharedPtr<O>&& other)
	: SharedPtr{ other.m_pCounter, other.m_pValue }
{
	MoveConstructor(other);
}
template<typename T>
inline SharedPtr<T>::SharedPtr(SharedPtr<T>&& other)
	: SharedPtr{ other.m_pCounter, other.m_pValue }
{
	MoveConstructor(other);
}
template<typename T>
template<typename O>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<O>& other)
{
	return CopyOperator(other);
}
template<typename T>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
	return CopyOperator(other);
}
template<typename T>
template<typename O>
inline SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<O>&& other)
{
	return MoveOperator(other);
}
template<typename T>
inline SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other)
{
	return MoveOperator(other);
}
template<typename T>
inline const T& SharedPtr<T>::Get() const
{
	return *m_pValue;
}
template<typename T>
inline T& SharedPtr<T>::Get()
{
	return *m_pValue;
}
}