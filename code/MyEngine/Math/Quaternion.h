#pragma once

#include "Matrices.h"
#include "Vectors.h"

namespace MyEngine
{
	namespace Math
	{
		class Quaternion
		{
		public:
			//---| Constructor / Destructor |---
			Quaternion();
			Quaternion(const Float3& real, float complex);
			explicit Quaternion(const Double4X4& matrix);
			~Quaternion() = default;
			static Quaternion FromAxis(const Float3& rotationAxis, float rotation);
			static Quaternion FromForward(const Float3& forward);
			static Quaternion FromEulerDegrees(const Float3& eulers);

			//---| Rule of five |---
			Quaternion(const Quaternion& other) = default;
			Quaternion(Quaternion&& other) noexcept = default;
			Quaternion& operator=(const Quaternion& other) = default;
			Quaternion& operator=(Quaternion&& other) noexcept = default;

			//---| XMVECTOR |---
			Quaternion(const DirectX::XMVECTOR& vector);
			void operator=(const DirectX::XMVECTOR& vector);

			//---| Operations |---
			void RotateBy(const Quaternion& rotation);
			void RotateBy(const Float3& axis, float radians);
			void RotatePoint(Float3& point) const;
			Float3 GetRotatedPoint(const Float3& point) const;

			Quaternion Normalized() const;
			void Normalize();

			Quaternion operator*(const Quaternion& other) const;
			void operator*=(const Quaternion& other);

			Quaternion operator-() const;
			void Inverse();

			//---| Get |---
			Float3 GetRight() const;
			Float3 GetUp() const;
			Float3 GetForward() const;
			float GetLength() const;

			//---| Data |---
			Float3 Xyz;
			float W;
		};
	}
}
