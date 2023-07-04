#pragma once
#include "Vector4.h"

namespace MyEngine
{
	template <typename T>
	class Matrix4X4
	{
	public:
		//---| Static |---
		static constexpr int NR_ROWS = 4;
		static constexpr int NR_COLS = 4;
		static Matrix4X4 GetIdentity();
		static Matrix4X4 FromRows(const Vector4<T>& row0, const Vector4<T>& row1, const Vector4<T>& row2, const Vector4<T>& row3);
		static Matrix4X4 FromCols(const Vector4<T>& col0, const Vector4<T>& col1, const Vector4<T>& col2, const Vector4<T>& col3);
		static Matrix4X4 FromRowMajor(const T* pValue);
		static Matrix4X4 FromColMajor(const T* pValue);

		//---| Constructor/Destructor |---
		Matrix4X4() = default;
		~Matrix4X4() = default;
		Matrix4X4(const Vector4<T>& col0, const Vector4<T>& col1, const Vector4<T>& col2, const Vector4<T>& col3);

		//---| Set |---
		void SetRow0(const Vector4<T>& row);
		void SetRow1(const Vector4<T>& row);
		void SetRow2(const Vector4<T>& row);
		void SetRow3(const Vector4<T>& row);
		void SetRow(int idx, const Vector4<T>& row);

		void SetRow0(const Vector3<T>& row);
		void SetRow1(const Vector3<T>& row);
		void SetRow2(const Vector3<T>& row);
		void SetRow3(const Vector3<T>& row);
		void SetRow(int idx, const Vector3<T>& row);

		void SetCol0(const Vector4<T>& col);
		void SetCol1(const Vector4<T>& col);
		void SetCol2(const Vector4<T>& col);
		void SetCol3(const Vector4<T>& col);
		void SetCol(int idx, const Vector4<T>& col);

		void SetCol0(const Vector3<T>& col);
		void SetCol1(const Vector3<T>& col);
		void SetCol2(const Vector3<T>& col);
		void SetCol3(const Vector3<T>& col);
		void SetCol(int idx, const Vector3<T>& col);

		//---| Get |---
		Vector4<T> GetRow0() const;
		Vector4<T> GetRow1() const;
		Vector4<T> GetRow2() const;
		Vector4<T> GetRow3() const;
		Vector4<T> GetRow(int idx) const;

		Vector4<T>& GetCol0();
		Vector4<T>& GetCol1();
		Vector4<T>& GetCol2();
		Vector4<T>& GetCol3();
		Vector4<T>& GetCol(int idx);

		const Vector4<T>& GetCol0() const;
		const Vector4<T>& GetCol1() const;
		const Vector4<T>& GetCol2() const;
		const Vector4<T>& GetCol3() const;
		const Vector4<T>& GetCol(int idx) const;

		//---| Operations |---
		Vector4<T>& operator[](int idx);
		const Vector4<T>& operator[](int idx) const;
		Matrix4X4 operator*(const Matrix4X4& r) const;
		Matrix4X4& operator*=(const Matrix4X4& r);

		Matrix4X4 GetTranspose() const;

	private:
		Vector4<T> m_Cols[4];
	};

	template <typename T>
	Matrix4X4<T>::Matrix4X4(const Vector4<T>& col0, const Vector4<T>& col1, const Vector4<T>& col2,
		const Vector4<T>& col3)
		: m_Cols{ col0, col1, col2, col3 }
	{
	}

	template <typename T>
	void Matrix4X4<T>::SetRow0(const Vector4<T>& row)
	{
		m_Cols[0].x = row.x;
		m_Cols[1].x = row.y;
		m_Cols[2].x = row.z;
		m_Cols[3].x = row.w;
	}

	template <typename T>
	void Matrix4X4<T>::SetRow1(const Vector4<T>& row)
	{
		m_Cols[0].y = row.x;
		m_Cols[1].y = row.y;
		m_Cols[2].y = row.z;
		m_Cols[3].y = row.w;
	}

	template <typename T>
	void Matrix4X4<T>::SetRow2(const Vector4<T>& row)
	{
		m_Cols[0].z = row.x;
		m_Cols[1].z = row.y;
		m_Cols[2].z = row.z;
		m_Cols[3].z = row.w;
	}

	template <typename T>
	void Matrix4X4<T>::SetRow3(const Vector4<T>& row)
	{
		m_Cols[0].w = row.x;
		m_Cols[1].w = row.y;
		m_Cols[2].w = row.z;
		m_Cols[3].w = row.w;
	}

	template <typename T>
	void Matrix4X4<T>::SetRow(int idx, const Vector4<T>& row)
	{
		m_Cols[0][idx] = row.x;
		m_Cols[1][idx] = row.y;
		m_Cols[2][idx] = row.z;
		m_Cols[3][idx] = row.w;
	}

	template <typename T>
	void Matrix4X4<T>::SetRow0(const Vector3<T>& row)
	{
		m_Cols[0].x = row.x;
		m_Cols[1].x = row.y;
		m_Cols[2].x = row.z;
	}

	template <typename T>
	void Matrix4X4<T>::SetRow1(const Vector3<T>& row)
	{
		m_Cols[0].y = row.x;
		m_Cols[1].y = row.y;
		m_Cols[2].y = row.z;
	}

	template <typename T>
	void Matrix4X4<T>::SetRow2(const Vector3<T>& row)
	{
		m_Cols[0].z = row.x;
		m_Cols[1].z = row.y;
		m_Cols[2].z = row.z;
	}

	template <typename T>
	void Matrix4X4<T>::SetRow3(const Vector3<T>& row)
	{
		m_Cols[0].w = row.x;
		m_Cols[1].w = row.y;
		m_Cols[2].w = row.z;
	}

	template <typename T>
	void Matrix4X4<T>::SetRow(int idx, const Vector3<T>& row)
	{
		m_Cols[0][idx] = row.x;
		m_Cols[1][idx] = row.y;
		m_Cols[2][idx] = row.z;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol0(const Vector4<T>& col)
	{
		m_Cols[0] = col;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol1(const Vector4<T>& col)
	{
		m_Cols[1] = col;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol2(const Vector4<T>& col)
	{
		m_Cols[2] = col;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol3(const Vector4<T>& col)
	{
		m_Cols[3] = col;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol(int idx, const Vector4<T>& col)
	{
		m_Cols[idx] = col;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol0(const Vector3<T>& col)
	{
		m_Cols[0] = col;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol1(const Vector3<T>& col)
	{
		m_Cols[1] = col;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol2(const Vector3<T>& col)
	{
		m_Cols[2] = col;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol3(const Vector3<T>& col)
	{
		m_Cols[3] = col;
	}

	template <typename T>
	void Matrix4X4<T>::SetCol(int idx, const Vector3<T>& col)
	{
		m_Cols[idx] = col;
	}

	template <typename T>
	Vector4<T> Matrix4X4<T>::GetRow0() const
	{
		return { m_Cols[0].x, m_Cols[1].x, m_Cols[2].x, m_Cols[3].x };
	}

	template <typename T>
	Vector4<T> Matrix4X4<T>::GetRow1() const
	{
		return { m_Cols[0].y, m_Cols[1].y, m_Cols[2].y, m_Cols[3].y };
	}

	template <typename T>
	Vector4<T> Matrix4X4<T>::GetRow2() const
	{
		return { m_Cols[0].z, m_Cols[1].z, m_Cols[2].z, m_Cols[3].z };
	}

	template <typename T>
	Vector4<T> Matrix4X4<T>::GetRow3() const
	{
		return { m_Cols[0].w, m_Cols[1].w, m_Cols[2].w, m_Cols[3].w };
	}

	template <typename T>
	Vector4<T> Matrix4X4<T>::GetRow(int idx) const
	{
		return { m_Cols[0][idx], m_Cols[1][idx], m_Cols[2][idx], m_Cols[3][idx] };
	}

	template <typename T>
	Vector4<T>& Matrix4X4<T>::GetCol0()
	{
		return m_Cols[0];
	}

	template <typename T>
	Vector4<T>& Matrix4X4<T>::GetCol1()
	{
		return m_Cols[1];
	}

	template <typename T>
	Vector4<T>& Matrix4X4<T>::GetCol2()
	{
		return m_Cols[2];
	}

	template <typename T>
	Vector4<T>& Matrix4X4<T>::GetCol3()
	{
		return m_Cols[3];
	}

	template <typename T>
	Vector4<T>& Matrix4X4<T>::GetCol(int idx)
	{
		return m_Cols[idx];
	}

	template <typename T>
	const Vector4<T>& Matrix4X4<T>::GetCol0() const
	{
		return m_Cols[0];
	}

	template <typename T>
	const Vector4<T>& Matrix4X4<T>::GetCol1() const
	{
		return m_Cols[1];
	}

	template <typename T>
	const Vector4<T>& Matrix4X4<T>::GetCol2() const
	{
		return m_Cols[2];
	}

	template <typename T>
	const Vector4<T>& Matrix4X4<T>::GetCol3() const
	{
		return m_Cols[3];
	}

	template <typename T>
	const Vector4<T>& Matrix4X4<T>::GetCol(int idx) const
	{
		return m_Cols[idx];
	}

	template <typename T>
	Vector4<T>& Matrix4X4<T>::operator[](int idx)
	{
		return m_Cols[idx];
	}

	template <typename T>
	const Vector4<T>& Matrix4X4<T>::operator[](int idx) const
	{
		return m_Cols[idx];
	}

	template <typename T>
	Matrix4X4<T> Matrix4X4<T>::operator*(const Matrix4X4& r) const
	{
		const Vector4<T> rows[4]{
			GetRow0(), GetRow1(), GetRow2(), GetRow3()
		};
		Matrix4X4 result{};
		result[0][0] = rows[0].Dot(r.GetCol0()); result[1][0] = rows[0].Dot(r.GetCol1());
		result[2][0] = rows[0].Dot(r.GetCol2()); result[3][0] = rows[0].Dot(r.GetCol3());

		result[0][1] = rows[1].Dot(r.GetCol0()); result[1][1] = rows[1].Dot(r.GetCol1());
		result[2][1] = rows[1].Dot(r.GetCol2()); result[3][1] = rows[1].Dot(r.GetCol3());

		result[0][2] = rows[2].Dot(r.GetCol0()); result[1][2] = rows[2].Dot(r.GetCol1());
		result[2][2] = rows[2].Dot(r.GetCol2()); result[3][2] = rows[2].Dot(r.GetCol3());

		result[0][3] = rows[3].Dot(r.GetCol0()); result[1][3] = rows[3].Dot(r.GetCol1());
		result[2][3] = rows[3].Dot(r.GetCol2()); result[3][3] = rows[3].Dot(r.GetCol3());
		return result;
	}

	template <typename T>
	Matrix4X4<T>& Matrix4X4<T>::operator*=(const Matrix4X4& r)
	{
		const Vector4<T> rows[4]{
			GetRow0(), GetRow1(), GetRow2(), GetRow3()
		};
		m_Cols[0].x = rows[0].Dot(r.GetCol0()); m_Cols[1].x = rows[0].Dot(r.GetCol1());
		m_Cols[2].x = rows[0].Dot(r.GetCol2()); m_Cols[3].x = rows[0].Dot(r.GetCol3());

		m_Cols[0].y = rows[1].Dot(r.GetCol0()); m_Cols[1].y = rows[1].Dot(r.GetCol1());
		m_Cols[2].y = rows[1].Dot(r.GetCol2()); m_Cols[3].y = rows[1].Dot(r.GetCol3());

		m_Cols[0].z = rows[2].Dot(r.GetCol0()); m_Cols[1].z = rows[2].Dot(r.GetCol1());
		m_Cols[2].z = rows[2].Dot(r.GetCol2()); m_Cols[3].z = rows[2].Dot(r.GetCol3());

		m_Cols[0].w = rows[3].Dot(r.GetCol0()); m_Cols[1].w = rows[3].Dot(r.GetCol1());
		m_Cols[2].w = rows[3].Dot(r.GetCol2()); m_Cols[3].w = rows[3].Dot(r.GetCol3());
		return *this;
	}

	template <typename T>
	Matrix4X4<T> Matrix4X4<T>::GetTranspose() const
	{
		return {
			{m_Cols[0].x, m_Cols[1].x, m_Cols[2].x, m_Cols[3].x },
			{m_Cols[0].y, m_Cols[1].y, m_Cols[2].y, m_Cols[3].y },
			{m_Cols[0].z, m_Cols[1].z, m_Cols[2].z, m_Cols[3].z },
			{m_Cols[0].w, m_Cols[1].w, m_Cols[2].w, m_Cols[3].w },
		};
	}

	template <typename T>
	Matrix4X4<T> Matrix4X4<T>::GetIdentity()
	{
		return {
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1}
		};
	}

	template <typename T>
	Matrix4X4<T> Matrix4X4<T>::FromRows(const Vector4<T>& row0, const Vector4<T>& row1, const Vector4<T>& row2,
		const Vector4<T>& row3)
	{
		return {
			{row0.x, row1.x, row2.x, row3.x },
			{row0.y, row1.y, row2.y, row3.y },
			{row0.z, row1.z, row2.z, row3.z },
			{row0.w, row1.w, row2.w, row3.w },
		};
	}

	template <typename T>
	Matrix4X4<T> Matrix4X4<T>::FromCols(const Vector4<T>& col0, const Vector4<T>& col1, const Vector4<T>& col2,
		const Vector4<T>& col3)
	{
		return { col0, col1, col2, col3 };
	}

	template <typename T>
	Matrix4X4<T> Matrix4X4<T>::FromRowMajor(const T* pValue)
	{
		return {
			{*(pValue + 4 * 0 + 0), *(pValue + 4 * 1 + 0), *(pValue + 4 * 2 + 0), *(pValue + 4 * 3 + 0) },
			{*(pValue + 4 * 0 + 1), *(pValue + 4 * 1 + 1), *(pValue + 4 * 2 + 1), *(pValue + 4 * 3 + 1) },
			{*(pValue + 4 * 0 + 2), *(pValue + 4 * 1 + 2), *(pValue + 4 * 2 + 2), *(pValue + 4 * 3 + 2) },
			{*(pValue + 4 * 0 + 3), *(pValue + 4 * 1 + 3), *(pValue + 4 * 2 + 3), *(pValue + 4 * 3 + 3) },
		};
	}

	template <typename T>
	Matrix4X4<T> Matrix4X4<T>::FromColMajor(const T* pValue)
	{
		return {
			{*(pValue + 4 * 0 + 0), *(pValue + 4 * 0 + 1), *(pValue + 4 * 0 + 2), *(pValue + 4 * 0 + 3)},
			{*(pValue + 4 * 1 + 0), *(pValue + 4 * 1 + 1), *(pValue + 4 * 1 + 2), *(pValue + 4 * 1 + 3)},
			{*(pValue + 4 * 2 + 0), *(pValue + 4 * 2 + 1), *(pValue + 4 * 2 + 2), *(pValue + 4 * 2 + 3)},
			{*(pValue + 4 * 3 + 0), *(pValue + 4 * 3 + 1), *(pValue + 4 * 3 + 2), *(pValue + 4 * 3 + 3)},
		};
	}
}
