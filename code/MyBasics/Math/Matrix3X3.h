#pragma once
#include "Vector3.h"

namespace MyEngine
{
	template<typename T>
	class Matrix3X3
	{
	public:
		//---| Static |---
		static constexpr unsigned NR_COLS = 3;
		static constexpr unsigned NR_ROWS = 3;
		static constexpr unsigned NR_ELEMENTS = NR_COLS * NR_ROWS;
		static constexpr unsigned COL_LENGTH = NR_ROWS;
		static constexpr unsigned ROW_LENGTH = NR_COLS;
		static Matrix3X3 Identity();

		//---| Construction |---
		Matrix3X3() = default;
		~Matrix3X3() = default;
		Matrix3X3(const Matrix3X3& other) = default;
		Matrix3X3(Matrix3X3&& other) noexcept = default;
		Matrix3X3& operator=(const Matrix3X3& other) = default;
		Matrix3X3& operator=(Matrix3X3&& other) noexcept = default;

		Matrix3X3(const Vector3<T>& col0, const Vector3<T>& col1, const Vector3<T>& col2);

		//---| Get/Set |---
		Vector3<T>& GetCol(unsigned idx);
		Vector3<T>& GetCol0();
		Vector3<T>& GetCol1();
		Vector3<T>& GetCol2();

		const Vector3<T>& GetCol(unsigned idx) const;
		const Vector3<T>& GetCol0() const;
		const Vector3<T>& GetCol1() const;
		const Vector3<T>& GetCol2() const;

	private:
		T m_Data[NR_ELEMENTS]; //column-major
	};

	template <typename T>
	Matrix3X3<T> Matrix3X3<T>::Identity()
	{
		Matrix3X3 m;
		m.m_Data =
		{
			1,0,0,
			0,1,0,
			0,0,1
		};
		return m;
	}

	template <typename T>
	Matrix3X3<T>::Matrix3X3(const Vector3<T>& col0, const Vector3<T>& col1, const Vector3<T>& col2)
	{
		GetCol0() = col0;
		GetCol1() = col1;
		GetCol2() = col2;
	}

	template <typename T>
	Vector3<T>& Matrix3X3<T>::GetCol(unsigned idx)
	{
		return reinterpret_cast<Vector3<T>&>(&m_Data[COL_LENGTH * idx]);
	}

	template <typename T>
	Vector3<T>& Matrix3X3<T>::GetCol0()
	{
		return GetCol(0);
	}

	template <typename T>
	Vector3<T>& Matrix3X3<T>::GetCol1()
	{
		return GetCol(1);
	}

	template <typename T>
	Vector3<T>& Matrix3X3<T>::GetCol2()
	{
		return GetCol(2);
	}

	template <typename T>
	const Vector3<T>& Matrix3X3<T>::GetCol(unsigned idx) const
	{
		return reinterpret_cast<Vector3<T>&>(&m_Data[COL_LENGTH * idx]);
	}

	template <typename T>
	const Vector3<T>& Matrix3X3<T>::GetCol0() const
	{
		return GetCol(0);
	}

	template <typename T>
	const Vector3<T>& Matrix3X3<T>::GetCol1() const
	{
		return GetCol(1);
	}

	template <typename T>
	const Vector3<T>& Matrix3X3<T>::GetCol2() const
	{
		return GetCol(2);
	}
}
