#pragma once

namespace MyEngine
{
	namespace Ds
	{
		template<typename Data>
		class Array
		{
		public:
			Array();
			Array(int size);
			Array(int size, const Data& initValue);
			~Array();
			Array(const Array& other);
			Array(Array&& other) noexcept;
			Array& operator=(const Array& other);
			Array& operator=(Array&& other) noexcept;

			void Add(const Array<Data>& other);

			const Data& operator[](int idx) const;
			Data& operator[](int idx);
			int GetSize() const { return m_Size; }
			Data* GetData() const { return m_pData; }

		private:
			Data* m_pData;
			int m_Size;
		};

		template <typename Data>
		Array<Data>::Array()
			: m_pData{ nullptr }
			, m_Size{ 0 }
		{
		}

		template <typename Data>
		Array<Data>::Array(Array&& other) noexcept
			: m_pData{other.m_pData}
			, m_Size{other.m_Size}
		{
			other.m_pData = nullptr;
			other.m_Size = 0;
		}

		template <typename Data>
		Array<Data>& Array<Data>::operator=(const Array& other)
		{
			if (m_pData) delete[] m_pData;
			m_Size = other.m_Size;
			m_pData = new Data[m_Size];
			for (int i = 0; i < m_Size; i++)
				m_pData[i] = other.m_pData[i];
			return *this;
		}

		template <typename Data>
		Array<Data>& Array<Data>::operator=(Array&& other) noexcept
		{
			if (m_pData) delete[] m_pData;
			m_pData = other.m_pData;
			m_Size = other.m_Size;
			other.m_pData = nullptr;
			other.m_Size = 0;
			return *this;
		}

		template <typename Data>
		Array<Data>::Array(int size)
			: m_pData(new Data[size])
			, m_Size(size)
		{
		}

		template <typename Data>
		Array<Data>::Array(int size, const Data& initValue)
			: m_pData(new Data[size])
			, m_Size(size)
		{
			for (int i = 0; i < m_Size; i++)
				m_pData[i] = initValue;
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
			for (int i = 0; i < m_Size; i++)
				m_pData[i] = other.m_pData[i];
		}

		template <typename Data>
		void Array<Data>::Add(const Array<Data>& other)
		{
			Data* pNew = new Data[m_Size + other.m_Size];
			for (int i = 0; i < m_Size; i++) pNew[i] = m_pData[i];
			for (int i = 0; i < other.m_Size; i++) pNew[i + m_Size] = other.m_pData[i];
			delete[] m_pData;
			m_pData = pNew;
			m_Size += other.m_Size;
		}

		template <typename Data>
		const Data& Array<Data>::operator[](int idx) const
		{
			return m_pData[idx];
		}

		template <typename Data>
		Data& Array<Data>::operator[](int idx)
		{
			return m_pData[idx];
		}
	}
}
