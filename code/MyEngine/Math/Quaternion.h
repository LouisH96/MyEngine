#pragma once
#include <Math/Float3.h>

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
			~Quaternion() = default;
			static Quaternion FromAxis(const Float3& rotationAxis, float rotation);

			//---| Rule of five |---
			Quaternion(const Quaternion& other) = default;
			Quaternion(Quaternion&& other) noexcept = default;
			Quaternion& operator=(const Quaternion& other) = default;
			Quaternion& operator=(Quaternion&& other) noexcept = default;

			//---| Operations |---
			Quaternion operator*(const Quaternion& other) const;
			void operator*=(const Quaternion& other);
			Quaternion Normalized() const;
			void Normalize();

			//---| Get |---
			const Float3& GetReal() const { return m_Real; }
			float GetComplex() const { return m_Complex; }

			Float3 GetRight() const;
			Float3 GetUp() const;
			Float3 GetForward() const;

			float GetLength() const;

		private:
			Float3 m_Real;
			float m_Complex;
		};
	}
}
