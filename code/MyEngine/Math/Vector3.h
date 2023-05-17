#pragma once
#include <DirectXMath.h>

#include "Vector2.h"

namespace MyEngine
{
	namespace Math
	{
		template<typename T>
		struct Vector3
		{
			Vector3();
			Vector3(T x, T y, T z);
			explicit Vector3(T all);
			~Vector3() = default;
			Vector3(const Vector3& other) = default;
			Vector3(Vector3&& other) noexcept = default;
			Vector3& operator=(const Vector3& other) = default;
			Vector3& operator=(Vector3&& other) noexcept = default;

			Vector3 operator+(const Vector3& r) const;
			Vector3 operator-(const Vector3& r) const;
			Vector3 operator+(const T& r) const;
			Vector3 operator-(const T& r) const;
			Vector3 operator*(const T& r) const;
			Vector3 operator/(const T& r) const;
			void operator+=(const Vector3& r);
			void operator-=(const Vector3& r);
			void operator+=(const T& r);
			void operator-=(const T& r);
			void operator*=(const T& r);
			void operator/=(const T& r);
			Vector3 operator-() const;

			T Get(int i) const;
			void Set(T f, int i);

			Vector2<T> Xz() const;

			void Reverse();
			void Scale(const Vector3& r);
			Vector3 Scaled(const Vector3& scale) const;
			T Length() const;
			T LengthSq() const;
			void Normalize();
			Vector3 Normalized() const;
			void Normalize(float& length);
			Vector3 Normalized(float& length) const;

			Vector3 Cross(const Vector3& other) const;
			float Dot(const Vector3& other) const;

			void LimitLength(T maxLength);
			Vector3 LimitedLength(T maxLength) const;

			static Vector3 RgbNormalize(const T& r, const T& g, const T& b);
			static Vector3 FromXz(const Math::Vector2<T>& xz);
			static Vector3 FromDxVector(const DirectX::XMVECTOR& vector);

			T x, y, z;
			static constexpr int NR_AXIS = 3;
		};

		template <typename T> Vector3<T>::Vector3() : x{ 0 }, y{ 0 }, z{ 0 } {}
		template <typename T> Vector3<T>::Vector3(T x, T y, T z) : x{ x }, y{ y }, z{ z } {}

		template <typename T> Vector3<T>::Vector3(T all) : x{ all }, y{ all }, z{ all } {}
		template <typename T> Vector3<T> Vector3<T>::operator+(const Vector3& r) const { return { x + r.x, y + r.y, z + r.z }; }
		template <typename T> Vector3<T> Vector3<T>::operator-(const Vector3& r) const { return { x - r.x, y - r.y, z - r.z }; }
		template <typename T> Vector3<T> Vector3<T>::operator+(const T& r) const { return { x + r, y + r, z + r }; }
		template <typename T> Vector3<T> Vector3<T>::operator-(const T& r) const { return { x - r, y - r, z - r }; }
		template <typename T> Vector3<T> Vector3<T>::operator*(const T& r) const { return { x * r, y * r, z * r }; }
		template <typename T>Vector3<T> Vector3<T>::operator/(const T& r) const { return { x / r, y / r, z / r }; }
		template <typename T> void Vector3<T>::operator+=(const Vector3& r) { x += r.x; y += r.y; z += r.z; }
		template <typename T> void Vector3<T>::operator-=(const Vector3& r) { x -= r.x; y -= r.y; z -= r.z; }
		template <typename T> void Vector3<T>::operator+=(const T& r) { x += r; y += r; z += r; }
		template <typename T> void Vector3<T>::operator-=(const T& r) { x -= r; y -= r; z -= r; }
		template <typename T> void Vector3<T>::operator*=(const T& r) { x *= r; y *= r; z *= r; }
		template <typename T>void Vector3<T>::operator/=(const T& r) { x /= r; y /= r; z /= r; }
		template <typename T>Vector3<T> Vector3<T>::operator-() const { return { -x, -y, -z }; }

		template <typename T>
		T Vector3<T>::Get(int i) const
		{
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		template <typename T>
		void Vector3<T>::Set(T f, int i)
		{
			if (i == 0) x = f;
			else if (i == 1) y = f;
			else z = f;
		}

		template <typename T>
		Vector2<T> Vector3<T>::Xz() const
		{
			return { x, z };
		}

		template <typename T>
		void Vector3<T>::Reverse()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		template <typename T>
		void Vector3<T>::Scale(const Vector3& r)
		{
			x *= r.x;
			y *= r.y;
			z *= r.z;
		}

		template <typename T>
		Vector3<T> Vector3<T>::Scaled(const Vector3& scale) const
		{
			return { x * scale.x, y * scale.y, z * scale.z };
		}

		template <typename T>
		T Vector3<T>::Length() const
		{
			return sqrtf(x * x + y * y + z * z);
		}

		template <typename T>
		T Vector3<T>::LengthSq() const
		{
			return x * x + y * y + z * z;
		}

		template <typename T>
		void Vector3<T>::Normalize()
		{
			const float scale = 1.f / Length();
			x *= scale;
			y *= scale;
			z *= scale;
		}

		template <typename T>
		Vector3<T> Vector3<T>::Normalized() const
		{
			const float scale = 1.f / Length();
			return { x * scale, y * scale, z * scale };
		}

		template <typename T>
		void Vector3<T>::Normalize(float& length)
		{
			length = Length();
			const float scale = 1.f / length;
			x *= scale;
			y *= scale;
			z *= scale;
		}

		template <typename T>
		Vector3<T> Vector3<T>::Normalized(float& length) const
		{
			length = Length();
			const float scale = 1.f / length;
			return { x * scale, y * scale, z * scale };
		}

		template <typename T>
		Vector3<T> Vector3<T>::Cross(const Vector3& other) const
		{
			return
			{
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			};
		}

		template <typename T>
		float Vector3<T>::Dot(const Vector3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		template <typename T>
		void Vector3<T>::LimitLength(T maxLength)
		{
			T value{ LengthSq() };
			if (value <= maxLength * maxLength) return;
			value = maxLength / sqrt(value);
			x *= value;
			y *= value;
			z *= value;
		}

		template <typename T>
		Vector3<T> Vector3<T>::LimitedLength(T maxLength) const
		{
			T value{ LengthSq() };
			if (value <= maxLength * maxLength) return *this;
			value = maxLength / sqrt(value);
			return{
				x * value,
				y * value,
				z * value
			};
		}

		template <typename T>
		Vector3<T> Vector3<T>::RgbNormalize(const T& r, const T& g, const T& b)
		{
			const T scale{ 1 / static_cast<T>(255) };
			return { r * scale, g * scale , b* scale};
		}

		template <typename T>
		Vector3<T> Vector3<T>::FromXz(const Math::Vector2<T>& xz)
		{
			return { xz.x, 0, xz.y };
		}

		template <typename T>
		Vector3<T> Vector3<T>::FromDxVector(const DirectX::XMVECTOR& vector)
		{
			Vector3 v{};
			XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&v.x), vector);
			return v;
		}
	}
}
