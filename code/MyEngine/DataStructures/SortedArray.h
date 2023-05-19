#pragma once
#include "SortedList.h"

namespace MyEngine
{
	namespace Ds
	{
		template<typename T>
		class SortedArray
		{
		public:
			//---| Constructor/Destructor |---
			SortedArray() = default;
			explicit SortedArray(const SortedList<T>& sortedList);
			explicit SortedArray(const Array<T>& unsortedArray);

			//---| Move/Copy |---
			SortedArray(const SortedArray& other) = default;
			SortedArray(SortedArray&& other) noexcept = default;
			SortedArray& operator=(const SortedArray& other) = default;
			SortedArray& operator=(SortedArray&& other) noexcept = default;

			//---| Functions |---
			void GetSurroundingValues(const T& value, const T*& before, const T*& after) const;
			void GetSurroundingIndices(const T& value, int& before, int& after) const;

			int GetSize() const { return m_Array.GetSize(); }
			const Array<T>& GetArray() const { return m_Array; }
			const T& operator[](int idx) const;

		private:
			Array<T> m_Array;
		};

		template <typename T>
		SortedArray<T>::SortedArray(const SortedList<T>& sortedList)
			: m_Array{ Array<T>::CopyFrom(sortedList.GetData(), sortedList.GetSize()) }
		{
		}

		template <typename T>
		SortedArray<T>::SortedArray(const Array<T>& unsortedArray)
			: m_Array{ unsortedArray }
		{
			//todo: sort
		}

		template <typename T>
		void SortedArray<T>::GetSurroundingIndices(const T& value, int& before, int& after) const
		{
			if (m_Array[0] > value)
			{
				before = -1;
				after = 0;
				return;
			}
			for (int i = 1; i < m_Array.GetSize() - 1; i++)
			{
				if (m_Array[i] <= value) continue;
				after = i--;
				before = i;
				return;
			}
			if(m_Array.Last() >= value)
			{
				after = m_Array.GetSize() - 1;
				before = after - 1;
				return;
			}
			before = m_Array.GetSize() - 1;
			after = -1;
		}

		template <typename T>
		const T& SortedArray<T>::operator[](int idx) const
		{
			return m_Array[idx];
		}

		template <typename T>
		void SortedArray<T>::GetSurroundingValues(const T& value, const T*& before, const T*& after) const
		{
			if (m_Array[0] > value)
			{
				before = nullptr;
				after = &m_Array[0];
				return;
			}
			for (int i = 1; i < m_Array.GetSize()-1; i++)
			{
				if (m_Array[i] <= value) continue;
				before = &m_Array[i - 1];
				after = &m_Array[i];
				return;
			}
			if(m_Array.Last() >= value)
			{
				after = m_Array.Last();
				before = m_Array[m_Array.GetSize() - 2];
				return;
			}
			before = &m_Array.Last();
			after = nullptr;
		}
	}
}
