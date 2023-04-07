#pragma once
//#define ARRAY_DEBUG

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
			Array(unsigned size);
			Array(unsigned size, const Data& initValue);
			~Array();
			Array(const Array& other);
			Array(Array&& other) noexcept;
			Array& operator=(const Array& other);
			Array& operator=(Array&& other) noexcept;

			void Add(const Array<Data>& other);
			void Add(const Data& data);

			const Data& operator[](int idx) const;
			Data& operator[](int idx);
			Data& Last();
			Data& First();
			const Data& Last() const;
			const Data& First() const;
			int GetSize() const { return m_Size; }
			Data* GetData() const { return m_pData; }

			void DeleteAll() const;

		private:
			Data* m_pData;
			int m_Size;

			void DoBoundsCheck(int idx) const;
			void PrintOutOfBounds(int idx) const;
		};

		template <typename Data>
		Array<Data>::Array()
			: m_pData{ nullptr }
			, m_Size{ 0 }
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

		template<typename Data>
		inline Array<Data>::Array(unsigned size)
			: m_pData(new Data[size])
			, m_Size(size)
		{
		}

		template<typename Data>
		inline Array<Data>::Array(unsigned size, const Data& initValue)
			: m_pData(new Data[size])
			, m_Size(size)
		{
			for (int i = 0; i < m_Size; i++)
				m_pData[i] = initValue;
		}

		template <typename Data>
		Array<Data>::~Array()
		{
			if (m_pData)
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
		void Array<Data>::Add(const Data& data)
		{
			Data* pNew = new Data[m_Size + 1];
			for (int i = 0; i < m_Size; i++)
				pNew[i] = m_pData[i];
			pNew[m_Size] = data;
			m_Size++;
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
		Data& Array<Data>::operator[](int idx)
		{
#ifdef ARRAY_DEBUG
			DoBoundsCheck(idx);
#endif
			return m_pData[idx];
		}

		template <typename Data>
		Data& Array<Data>::Last()
		{
			return m_pData[m_Size-1];
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
		void Array<Data>::DeleteAll() const
		{
			for (int i = 0; i < m_Size; i++)
				delete m_pData[i];
		}

		template <typename Data>
		void Array<Data>::DoBoundsCheck(int idx) const
		{
			if (idx < 0 || idx >= m_Size)
				PrintOutOfBounds(idx);
		}

		template <typename Data>
		void Array<Data>::PrintOutOfBounds(int idx) const
		{
			std::cout << "ArrayIdx " << idx
				<< " out of bounds[0-" << m_Size - 1 << "]\n";
		}
	}
}
