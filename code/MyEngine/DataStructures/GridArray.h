#pragma once


namespace MyEngine
{
	template<typename T>
	class GridArray
	{
	public:
		//---| Constructor/Destructor |---
		GridArray();
		explicit GridArray(Int2 size);
		explicit GridArray(Int2 size, const T& initValue);
		~GridArray();

		//---| Rule of five |---
		GridArray(const GridArray& other);
		GridArray(GridArray&& other) noexcept;
		GridArray& operator=(const GridArray& other);
		GridArray& operator=(GridArray&& other) noexcept;

		//---| Operations |---
		void Set(Int2 pos, const T& data);
		void Set(int idx, const T& data);
		const T& Get(Int2 pos) const;
		const T& Get(int idx) const;
		T& Get(Int2 pos);
		T& Get(int idx);
		int PosToIdx(Int2 pos) const;
		Int2 IdxToPos(int idx) const;

		int GetNrCols() const { return m_Size.x; }
		int GetNrRows() const { return m_Size.y; }
		Int2 GetSize() const { return m_Size; }
		int GetNrElements() const { return m_Size.x * m_Size.y; }

	private:
		Int2 m_Size; //{nrCols,nrRows}
		T* m_pData;
	};

	template <typename T>
	GridArray<T>::GridArray() : m_Size{ 0,0 }, m_pData{ nullptr }{}

	template <typename T>
	GridArray<T>::GridArray(Int2 size)
		: m_Size{ size }
		, m_pData{ new T[size.x * size.y] }
	{
	}

	template <typename T>
	GridArray<T>::GridArray(Int2 size, const T& initValue)
		: m_Size{ size }
		, m_pData{ new T[size.x * size.y] }
	{
		for (int i = 0; i < size.x * size.y; i++)
			m_pData[i] = initValue;
	}

	template <typename T>
	void GridArray<T>::Set(Int2 pos, const T& data)
	{
		m_pData[pos.y * m_Size.x + pos.x] = data;
	}

	template <typename T>
	const T& GridArray<T>::Get(Int2 pos) const
	{
		return m_pData[pos.y * m_Size.x + pos.x];
	}

	template <typename T>
	T& GridArray<T>::Get(Int2 pos)
	{
		return m_pData[pos.y * m_Size.x + pos.x];
	}

	template <typename T>
	GridArray<T>::~GridArray()
	{
		delete[] m_pData;
		m_Size = { 0,0 };
	}

	template <typename T>
	GridArray<T>::GridArray(const GridArray& other)
		: m_Size{ other.m_Size }
		, m_pData{ new T[m_Size.x * m_Size.y] }
	{
		memcpy(m_pData, other.m_pData, m_Size.x* m_Size.y * sizeof(T));
	}

	template <typename T>
	GridArray<T>::GridArray(GridArray&& other) noexcept
		: m_Size{ other.m_Size }
		, m_pData{ other.m_pData }
	{
		other.m_Size = { 0,0 };
		other.m_pData = nullptr;
	}

	template <typename T>
	GridArray<T>& GridArray<T>::operator=(const GridArray& other)
	{
		if (&other == this) return *this;
		m_Size = other.m_Size;
		m_pData = new T[m_Size.x * m_Size.y];
		memcpy(m_pData, other.m_pData, m_Size.x * m_Size.y * sizeof(T));
		return *this;
	}

	template <typename T>
	GridArray<T>& GridArray<T>::operator=(GridArray&& other) noexcept
	{
		if (&other == this) return *this;
		m_Size = other.m_Size;
		m_pData = other.m_pData;
		other.m_Size = { 0,0 };
		other.m_pData = nullptr;
		return *this;
	}

	template <typename T>
	void GridArray<T>::Set(int idx, const T& data)
	{
		m_pData[idx] = data;
	}

	template <typename T>
	const T& GridArray<T>::Get(int idx) const
	{
		return m_pData[idx];
	}

	template <typename T>
	T& GridArray<T>::Get(int idx)
	{
		return m_pData[idx];
	}

	template <typename T>
	int GridArray<T>::PosToIdx(Int2 pos) const
	{
		return pos.y * m_Size.x + pos.x;
	}

	template <typename T>
	Int2 GridArray<T>::IdxToPos(int idx) const
	{
		return { idx % m_Size.x, idx / m_Size.x };
	}
}
