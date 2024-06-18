#pragma once
#include <algorithm>

#include "Math/Vectors.h"

namespace MyEngine
{
	template<typename T>
	class CubeArray
	{
	public:
		//---| Construction |---
		CubeArray();
		explicit CubeArray(const Int3& size);
		~CubeArray();

		CubeArray(const CubeArray& other);
		CubeArray& operator=(const CubeArray& other);
		CubeArray(CubeArray&& other) noexcept;
		CubeArray& operator=(CubeArray&& other) noexcept;

		//---| Functions |---
		const T& Get(const Int3& pos) const;
		T& Get(const Int3& pos);

		const T* GetSafe(const Int3& pos) const;
		T* GetSafe(const Int3& pos);

		const T& operator[](const Int3& pos) const;
		T& operator[](const Int3& pos);

		const T& operator[](unsigned idx) const;
		T& operator[](unsigned idx);

		void Set(const Int3& pos, const T& data);

		const Int3& GetSize() const { return m_Size; }
		unsigned GetCount() const { return Count(m_Size); }
		unsigned GetNrCols() const { return m_Size.x; }
		unsigned GetNrRows() const { return m_Size.y; }
		unsigned GetNrDepths() const { return m_Size.z; }

		const T* GetData() const { return m_pData; }
		T* GetData() { return m_pData; }

		const T* GetEnd() const { return m_pData[GetCount()]; }
		T* GetEnd() { return m_pData[GetCount()]; }

	private:
		Int3 m_Size;
		T* m_pData;

		static unsigned Count(const Int3& size);
	};

	template <typename T>
	CubeArray<T>::CubeArray()
		: m_Size{}
		, m_pData{ nullptr }
	{
	}

	template <typename T>
	CubeArray<T>::CubeArray(const Int3& size)
		: m_Size{ size }
		, m_pData{ new T[Count(size)] }
	{
	}

	template <typename T>
	CubeArray<T>::~CubeArray()
	{
		m_Size = {};
		delete[] m_pData;
		m_pData = nullptr;
	}

	template <typename T>
	CubeArray<T>::CubeArray(const CubeArray& other)
		: m_Size{ other.m_Size }
		, m_pData{ new T[Count(other.m_Size)] }
	{
		std::copy(other.m_pData, other.GetEnd(), m_pData);
	}

	template <typename T>
	CubeArray<T>& CubeArray<T>::operator=(const CubeArray& other)
	{
		if (&other == this) return *this;
		m_Size = other.m_Size;
		delete[] m_pData;
		m_pData = new T[GetCount()];
		std::copy(other.m_pData, other.GetEnd(), m_pData);
		return *this;
	}

	template <typename T>
	CubeArray<T>::CubeArray(CubeArray&& other) noexcept
		: m_Size{ other.m_Size }
		, m_pData{ other.m_pData }
	{
		other.m_Size = {};
		other.m_pData = nullptr;
	}

	template <typename T>
	CubeArray<T>& CubeArray<T>::operator=(CubeArray&& other) noexcept
	{
		if (&other == this) return *this;
		delete[] m_pData;
		m_Size = other.m_Size;
		m_pData = other.m_pData;
		other.m_Size = {};
		other.m_pData = nullptr;
		return *this;
	}

	template <typename T>
	const T& CubeArray<T>::Get(const Int3& pos) const
	{
		return m_pData[pos.x +
			m_Size.x * pos.y +
			m_Size.x * m_Size.y * pos.z];
	}

	template <typename T>
	T& CubeArray<T>::Get(const Int3& pos)
	{
		return m_pData[pos.x +
			m_Size.x * pos.y +
			m_Size.x * m_Size.y * pos.z];
	}

	template <typename T>
	const T* CubeArray<T>::GetSafe(const Int3& pos) const
	{
		if (pos.x < 0 || pos.x >= m_Size.x ||
			pos.y < 0 || pos.y >= m_Size.y ||
			pos.z < 0 || pos.z >= m_Size.z) return nullptr;
		return &m_pData[pos.x +
			m_Size.x * pos.y +
			m_Size.x * m_Size.y * pos.z];
	}

	template <typename T>
	T* CubeArray<T>::GetSafe(const Int3& pos)
	{
		if (pos.x < 0 || pos.x >= m_Size.x ||
			pos.y < 0 || pos.y >= m_Size.y ||
			pos.z < 0 || pos.z >= m_Size.z) return nullptr;
		return &m_pData[pos.x +
			m_Size.x * pos.y +
			m_Size.x * m_Size.y * pos.z];
	}

	template <typename T>
	const T& CubeArray<T>::operator[](const Int3& pos) const
	{
		return m_pData[pos.x +
			m_Size.x * pos.y +
			m_Size.x * m_Size.y * pos.z];
	}

	template <typename T>
	T& CubeArray<T>::operator[](const Int3& pos)
	{
		return m_pData[pos.x +
			m_Size.x * pos.y +
			m_Size.x * m_Size.y * pos.z];
	}

	template <typename T>
	const T& CubeArray<T>::operator[](unsigned idx) const
	{
		return m_pData[idx];
	}

	template <typename T>
	T& CubeArray<T>::operator[](unsigned idx)
	{
		return m_pData[idx];
	}

	template <typename T>
	void CubeArray<T>::Set(const Int3& pos, const T& data)
	{
		m_pData[pos.x +
			m_Size.x * pos.y +
			m_Size.x * m_Size.y * pos.z] = data;
	}

	template <typename T>
	unsigned CubeArray<T>::Count(const Int3& size)
	{
		return size.x * size.y * size.z;
	}
}
