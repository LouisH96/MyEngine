#pragma once

#include "Matrices.h"
#include "Vectors.h"

namespace MyEngine
{
	class Quaternion
	{
	public:
		//---| Constructor / Destructor |---
		Quaternion();
		Quaternion(const Float3& real, float complex);
		explicit Quaternion(const Double4X4& matrix);
		explicit Quaternion(const Float4X4& matrix);
		~Quaternion() = default;
		static Quaternion FromAxis(const Float3& rotationAxis, float rotation);
		static Quaternion FromForward(const Float3& forward);
		static Quaternion FromEulerDegrees(const Float3& eulers);

		//---| Rule of five |---
		Quaternion(const Quaternion& other) = default;
		Quaternion(Quaternion&& other) noexcept = default;
		Quaternion& operator=(const Quaternion& other) = default;
		Quaternion& operator=(Quaternion&& other) noexcept = default;

		//---| Operations |---
		void RotateBy(const Quaternion& rotation);
		void RotateBy(const Float3& axis, float radians);
		void RotatePoint(Float3& point) const;
		Float3 GetRotatedPoint(const Float3& point) const;
		static float Dot(const Quaternion& a, const Quaternion& b);
		static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
		static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);

		Quaternion Normalized() const;
		void Normalize();

		Quaternion operator+(const Quaternion& other) const;
		void operator+=(const Quaternion& other);

		Quaternion operator*(const Quaternion& other) const;
		void operator*=(const Quaternion& other);

		Quaternion operator*(float scale) const;
		void operator*=(float scale);

		Quaternion operator-() const;
		void Inverse();

		//---| Get |---
		Float3 GetRight() const;
		Float3 GetUp() const;
		Float3 GetForward() const;
		float GetLength() const;
		Float3 GetRotationAxis() const;
		Float4X4 AsMatrix() const;

		//---| Data |---
		Float3 Xyz;
		float W;

		Float4 AsVector() const { return { Xyz.x, Xyz.y, Xyz.z, W }; }
	};
}
