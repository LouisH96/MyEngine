#pragma once
#include "Vector3.h"

namespace MyEngine
{
	template<typename T>
	class DxMatrix3X3
	{
	public:
		//---| Static |---
		static constexpr unsigned NR_COLS = 3;
		static constexpr unsigned NR_ROWS = 3;
		static constexpr unsigned NR_ELEMENTS = NR_COLS * NR_ROWS;
		static constexpr unsigned COL_LENGTH = NR_ROWS;
		static constexpr unsigned ROW_LENGTH = NR_COLS;

		static constexpr unsigned INT_NR_COLS = 3;
		static constexpr unsigned INT_NR_ROWS = 4;
		static constexpr unsigned INT_NR_ELEMENTS = INT_NR_COLS * INT_NR_ROWS;
		static constexpr unsigned INT_COL_LENGTH = INT_NR_ROWS;
		static constexpr unsigned INT_ROW_LENGTH = INT_NR_COLS;
		static constexpr DxMatrix3X3 Identity();

		//---| Construction |---
		DxMatrix3X3() = default;
		~DxMatrix3X3() = default;
		DxMatrix3X3(const DxMatrix3X3& other) = default;
		DxMatrix3X3(DxMatrix3X3&& other) noexcept = default;
		DxMatrix3X3& operator=(const DxMatrix3X3& other) = default;
		DxMatrix3X3& operator=(DxMatrix3X3&& other) noexcept = default;

		DxMatrix3X3(const Vector3<T>& col0, const Vector3<T>& col1, const Vector3<T>& col2);

		//---| Get/Set |---
		Vector3<T>& GetCol(unsigned idx);
		Vector3<T>& GetCol0();
		Vector3<T>& GetCol1();
		Vector3<T>& GetCol2();

		Vector2<T>& GetV2Col(unsigned idx);
		Vector2<T>& GetV2Col0();
		Vector2<T>& GetV2Col1();
		Vector2<T>& GetV2Col2();

		const Vector3<T>& GetCol(unsigned idx) const;
		const Vector3<T>& GetCol0() const;
		const Vector3<T>& GetCol1() const;
		const Vector3<T>& GetCol2() const;

		const Vector2<T>& GetV2Col(unsigned idx) const;
		const Vector2<T>& GetV2Col0() const;
		const Vector2<T>& GetV2Col1() const;
		const Vector2<T>& GetV2Col2() const;

		Vector2<T> GetV2Row(unsigned row) const;
		Vector2<T> GetV2Row0() const;
		Vector2<T> GetV2Row1() const;
		Vector2<T> GetV2Row2() const;

		void SetV2Row(unsigned rowIdx, const Vector2<T>& row);
		void SetV2Row0(const Vector2<T>& row);
		void SetV2Row1(const Vector2<T>& row);
		void SetV2Row2(const Vector2<T>& row);

		T* Get() { return m_Data; }
		void Set(unsigned col, unsigned row, T value);

		float Get(unsigned col, unsigned row) const;
		const T* Get() const;

	private:
		T m_Data[INT_NR_ELEMENTS]; //column-major
	};

	template <typename T>
	DxMatrix3X3<T>::DxMatrix3X3(const Vector3<T>& col0, const Vector3<T>& col1, const Vector3<T>& col2)
	{
		GetCol0() = col0;
		GetCol1() = col1;
		GetCol2() = col2;
	}

	template <typename T>
	Vector3<T>& DxMatrix3X3<T>::GetCol(unsigned idx)
	{
		return reinterpret_cast<Vector3<T>&>(&m_Data[INT_COL_LENGTH * idx]);
	}

	template <typename T>
	Vector3<T>& DxMatrix3X3<T>::GetCol0()
	{
		return GetCol(0);
	}

	template <typename T>
	Vector3<T>& DxMatrix3X3<T>::GetCol1()
	{
		return GetCol(1);
	}

	template <typename T>
	Vector3<T>& DxMatrix3X3<T>::GetCol2()
	{
		return GetCol(2);
	}

	template <typename T>
	const Vector3<T>& DxMatrix3X3<T>::GetCol(unsigned idx) const
	{
		return reinterpret_cast<Vector3<T>&>(&m_Data[INT_COL_LENGTH * idx]);
	}

	template <typename T>
	const Vector3<T>& DxMatrix3X3<T>::GetCol0() const
	{
		return GetCol(0);
	}

	template <typename T>
	const Vector3<T>& DxMatrix3X3<T>::GetCol1() const
	{
		return GetCol(1);
	}

	template <typename T>
	const Vector3<T>& DxMatrix3X3<T>::GetCol2() const
	{
		return GetCol(2);
	}

	template <typename T>
	Vector2<T>& DxMatrix3X3<T>::GetV2Col(unsigned idx)
	{
		return reinterpret_cast<Vector2<T>&>(&m_Data[INT_COL_LENGTH * idx]);
	}

	template <typename T>
	Vector2<T>& DxMatrix3X3<T>::GetV2Col0()
	{
		return GetV2Col(0);
	}

	template <typename T>
	Vector2<T>& DxMatrix3X3<T>::GetV2Col1()
	{
		return GetV2Col(1);
	}

	template <typename T>
	Vector2<T>& DxMatrix3X3<T>::GetV2Col2()
	{
		return GetV2Col(2);
	}

	template <typename T>
	const Vector2<T>& DxMatrix3X3<T>::GetV2Col(unsigned idx) const
	{
		return reinterpret_cast<Vector2<T>&>(&m_Data[INT_COL_LENGTH * idx]);
	}

	template <typename T>
	const Vector2<T>& DxMatrix3X3<T>::GetV2Col0() const
	{
		return GetV2Col(0);
	}

	template <typename T>
	const Vector2<T>& DxMatrix3X3<T>::GetV2Col1() const
	{
		return GetV2Col(1);
	}

	template <typename T>
	const Vector2<T>& DxMatrix3X3<T>::GetV2Col2() const
	{
		return GetV2Col(2);
	}

	template <typename T>
	Vector2<T> DxMatrix3X3<T>::GetV2Row(unsigned row) const
	{
		return { m_Data[row], m_Data[INT_COL_LENGTH + row] };
	}

	template <typename T>
	Vector2<T> DxMatrix3X3<T>::GetV2Row0() const
	{
		return GetV2Row(0);
	}

	template <typename T>
	Vector2<T> DxMatrix3X3<T>::GetV2Row1() const
	{
		return GetV2Row(1);
	}

	template <typename T>
	Vector2<T> DxMatrix3X3<T>::GetV2Row2() const
	{
		return GetV2Row(2);
	}

	template <typename T>
	void DxMatrix3X3<T>::SetV2Row(unsigned rowIdx, const Vector2<T>& row)
	{
		m_Data[rowIdx] = row.x;
		m_Data[INT_COL_LENGTH + rowIdx] = row.y;
	}

	template <typename T>
	void DxMatrix3X3<T>::SetV2Row0(const Vector2<T>& row)
	{
		SetV2Row(0, row);
	}

	template <typename T>
	void DxMatrix3X3<T>::SetV2Row1(const Vector2<T>& row)
	{
		SetV2Row(1, row);
	}

	template <typename T>
	void DxMatrix3X3<T>::SetV2Row2(const Vector2<T>& row)
	{
		SetV2Row(2, row);
	}

	template <typename T>
	void DxMatrix3X3<T>::Set(unsigned col, unsigned row, T value)
	{
		m_Data[col * INT_COL_LENGTH + row] = value;
	}

	template <typename T>
	float DxMatrix3X3<T>::Get(unsigned col, unsigned row) const
	{
		return m_Data[col * INT_COL_LENGTH + row];
	}

	template <typename T>
	const T* DxMatrix3X3<T>::Get() const
	{
		return m_Data;
	}

	template <typename T>
	constexpr DxMatrix3X3<T> DxMatrix3X3<T>::Identity()
	{
		DxMatrix3X3 m;
		m.m_Data[0] = 1; m.m_Data[1] = 0; m.m_Data[2] = 0; m.m_Data[3] = 0;
		m.m_Data[4] = 0; m.m_Data[5] = 1; m.m_Data[6] = 0; m.m_Data[7] = 0;
		m.m_Data[8] = 0; m.m_Data[9] = 0; m.m_Data[10] = 1; m.m_Data[11] = 0;
		return m;
	}
}
