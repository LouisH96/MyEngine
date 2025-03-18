#pragma once
#include "Math/Matrices.h"
#include "Math/Vectors.h"
#include "Math/Quaternion.h"

namespace MyEngine
{
	//should have no scale
	class WorldMatrix
	{
	public:
		//---| Float4X4 |---
		static Float3 GetPosition(const Float4X4& world);
		static float GetXPosition(const Float4X4& world);
		static float GetYPosition(const Float4X4& world);
		static float GetZPosition(const Float4X4& world);

		static Float2 GetPositionXz(const Float4X4& world);
		static void SetPositionXz(Float4X4& world, const Float2& xz);

		static void SetRotation(Float4X4& world, const Float3& forward);
		static Float3 GetRight(const Float4X4& world);
		static Float3 GetUp(const Float4X4& world);
		static Float3 GetForward(const Float4X4& world);
		static Float2 GetForwardXz(const Float4X4& world);

		static float GetYaw(const Float4X4& world);
		static void SetYaw(Float4X4& world, float yaw);

		static void SetPosition(Float4X4& world, const Float3& position);
		static Float4X4 Translation(const Float3& translation);
		static Float4X4 Rotation(const Float3& forward);
		static Float4X4 Rotation(float yaw, float pitch);

		static void Translate(Float4X4& m, const Float3& translation);

		static void TranslateRelative(Float4X4& m, const Float3& translation);
		static void TranslateRelativeXz(Float4X4& m, const Float2& translation);
		static void TranslateRelativeX(Float4X4& m, float translation);
		static void TranslateRelativeY(Float4X4& m, float translation);

		static void SetPitchRotation(Float4X4& m, float pitch);

		static Float4X4 GetIdentity();

		template<typename T>
		static void TransformPoint(const Matrix4X4<T>& matrix, Vector3<T>& point);

		template<typename T>
		static Vector3<T> RotatePoint(const Matrix4X4<T>& matrix, const Vector3<T>& point);

		template<typename T> static void Inverse(Matrix4X4<T>& matrix);
		template<typename T> static Matrix4X4<T> GetInversed(const Matrix4X4<T>& matrix);

		static Float4X4 FromPosAndQuat(const Float3& pos, const Quaternion& quat);

		//---| DxMatrix3X3 |---
		template<typename T> static DxMatrix3X3<T> CreateDx3X3(const Vector2<T>& position);
		template<typename T> static DxMatrix3X3<T> CreateDx3X3(const Vector2<T>& position, T angle);

		template<typename T> static Vector2<T> GetPosition(const DxMatrix3X3<T>& transform);
		template<typename T> static T GetPositionZ(const DxMatrix3X3<T>& transform);

		template<typename T> static void SetPosition(DxMatrix3X3<T>& transform, const Vector2<T>& position);
		template<typename T> static void SetPositionZ(DxMatrix3X3<T>& transform, T posZ);

		template<typename T> static void SetRotation(DxMatrix3X3<T>& transform, T angle);
	};

	//row-vector * col-major-matrix
	template <typename T>
	void WorldMatrix::TransformPoint(const Matrix4X4<T>& matrix, Vector3<T>& point)
	{
		point = Vector3<T>{
			reinterpret_cast<const Vector3<T>&>(matrix.GetCol0()).Dot(point),
			reinterpret_cast<const Vector3<T>&>(matrix.GetCol1()).Dot(point),
			reinterpret_cast<const Vector3<T>&>(matrix.GetCol2()).Dot(point) }
		+ reinterpret_cast<const Vector3<T>&>(matrix.GetRow3());
	}

	template <typename T>
	void WorldMatrix::RotatePoint(const Matrix4X4<T>& matrix, Vector3<T>& point)
	{
		point = Vector3<T>{
			reinterpret_cast<const Vector3<T>&>(matrix.GetCol0()).Dot(point),
			reinterpret_cast<const Vector3<T>&>(matrix.GetCol1()).Dot(point),
			reinterpret_cast<const Vector3<T>&>(matrix.GetCol2()).Dot(point)
		};
	}

	//cannot handle scaling
	template <typename T>
	void WorldMatrix::Inverse(Matrix4X4<T>& matrix)
	{
		//0 & 1
		float temp = matrix.Get(0, 1);
		matrix.Set(0, 1, matrix.Get(1, 0));
		matrix.Set(1, 0, temp);

		//0 & 2
		temp = matrix.Get(0, 2);
		matrix.Set(0, 2, matrix.Get(2, 0));
		matrix.Set(2, 0, temp);

		//1 & 2
		temp = matrix.Get(1, 2);
		matrix.Set(1, 2, matrix.Get(2, 1));
		matrix.Set(2, 1, temp);

		matrix.SetRow3(-RotatePoint(matrix, matrix.GetRow3().Xyz()));
	}

	template <typename T>
	Matrix4X4<T> WorldMatrix::GetInversed(const Matrix4X4<T>& matrix)
	{
		Matrix4X4<T> inv
		{
			{ matrix.Get(0,0), matrix.Get(1,0), matrix.Get(2,0)},
			{ matrix.Get(0,1),matrix.Get(1,1),matrix.Get(2,1)},
			{ matrix.Get(0,2), matrix.Get(1,2),matrix.Get(2,2)}
		};
		inv.SetRow3(-RotatePoint(inv, matrix.GetRow3().Xyz()));
		inv.Set(3, 3, 1);
		return inv;
	}

	template <typename T>
	DxMatrix3X3<T> WorldMatrix::CreateDx3X3(const Vector2<T>& position)
	{
		DxMatrix3X3<T> m{ DxMatrix3X3<T>::Identity() };
		SetPosition(m, position);
		return m;
	}

	template <typename T>
	DxMatrix3X3<T> WorldMatrix::CreateDx3X3(const Vector2<T>& position, T angle)
	{
		DxMatrix3X3<T> m{ DxMatrix3X3<T>::Identity() };
		SetPosition(m, position);
		SetRotation(m, angle);
		return m;
	}

	template <typename T>
	Vector2<T> WorldMatrix::GetPosition(const DxMatrix3X3<T>& transform)
	{
		return transform.GetV2Row2();
	}

	template <typename T>
	T WorldMatrix::GetPositionZ(const DxMatrix3X3<T>& transform)
	{
		return transform.Get()[3 * DxFloat3X3::INT_COL_LENGTH + 3];
	}

	template <typename T>
	void WorldMatrix::SetPosition(DxMatrix3X3<T>& transform, const Vector2<T>& position)
	{
		transform.SetV2Row2(position);
	}

	template <typename T>
	void WorldMatrix::SetPositionZ(DxMatrix3X3<T>& transform, T posZ)
	{
		transform.Get()[3 * DxFloat3X3::INT_COL_LENGTH + 3] = posZ;
	}

	template <typename T>
	void WorldMatrix::SetRotation(DxMatrix3X3<T>& transform, T angle)
	{
		const float c{ cos(angle) };
		const float s{ sin(angle) };
		transform.Set(0, 0, c);
		transform.Set(0, 1, s);
		transform.Set(1, 0, -s);
		transform.Set(1, 1, c);
		;
	}
}
