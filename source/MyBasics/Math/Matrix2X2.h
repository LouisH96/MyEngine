// ReSharper disable CppInconsistentNaming
#pragma once

namespace MyEngine
{
	template<typename T>
	class Matrix2X2
	{
	public:
		//---| Static |---
		static constexpr unsigned NR_COLS = 2;
		static constexpr unsigned NR_ROWS = 2;
		static constexpr unsigned NR_ELEMENTS = NR_COLS * NR_ROWS;
		static constexpr unsigned COL_LENGTH = 2;
		static constexpr unsigned ROW_LENGTH = 2;
		static Matrix2X2 GetIdentity();

		//---| Construction |---
		Matrix2X2() = default;
		~Matrix2X2() = default;
		Matrix2X2(const Matrix2X2& other) = default;
		Matrix2X2(Matrix2X2&& other) noexcept = default;
		Matrix2X2& operator=(const Matrix2X2& other) = default;
		Matrix2X2& operator=(Matrix2X2&& other) noexcept = default;

		Matrix2X2(const T& c0r0, const T& c0r1, const T& c1r0, const T& c1r1);

		//---| Functions |---
		void Set(unsigned col, unsigned row, const T& value);
		const T& Get(unsigned col, unsigned row);

	private:
		T m_Data[8]; //column-major
	};

	template <typename T>
	Matrix2X2<T> Matrix2X2<T>::GetIdentity()
	{
		return { 1,0,0,1 };
	}

	template <typename T>
	Matrix2X2<T>::Matrix2X2(const T& c0r0, const T& c0r1, const T& c1r0, const T& c1r1)
		: m_Data{ c0r0, c0r1,0,0, c1r0, c1r1,0,0 }
	{
	}

	template <typename T>
	void Matrix2X2<T>::Set(unsigned col, unsigned row, const T& value)
	{
		m_Data[col * COL_LENGTH + row] = value;
	}

	template <typename T>
	const T& Matrix2X2<T>::Get(unsigned col, unsigned row)
	{
		return m_Data[col * COL_LENGTH + row];
	}
}
