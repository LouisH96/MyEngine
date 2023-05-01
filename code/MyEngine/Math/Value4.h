#pragma once
#include "Value3.h"

namespace MyEngine
{
	namespace Math
	{
		template<typename T>
		struct Value4
		{
			//---| Fields |---
			T x, y, z, w;

			//---| Constructor/Destructor |---
			Value4();
			Value4(T x, T y, T z, T w);
			Value4(const Value3<T>& vector3, T w);

			//---| Rule of five |---
			Value4(const Value4& other) = default;
			Value4(Value4&& other) noexcept = default;
			Value4& operator=(const Value4& other) = default;
			Value4& operator=(Value4&& other) noexcept = default;

			//---| Operations |---
			Value4 operator+(const Value4& r) const;
			Value4 operator-(const Value4& r) const;
			Value4 operator+(const T& r) const;
			Value4 operator-(const T& r) const;
			Value4 operator*(const T& r) const;
			Value4 operator/(const T& r) const;
			void operator+=(const Value4& r);
			void operator-=(const Value4& r);
			void operator+=(const T& r);
			void operator-=(const T& r);
			void operator*=(const T& r);
			void operator/=(const T& r);
			Value4 operator-() const;
			T& operator[](int idx);
			T operator[](int idx) const;

			T Get(int i) const;
			void Set(T f, int i);
			void Inverse();
			void Scale(const Value4& r);
			T Length() const;
			T LengthSq() const;
			T Dot(const Value4& other) const;
			void Normalize();
			Value4 Normalized() const;
		};

		template <typename T> Value4<T>::Value4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
		template <typename T> Value4<T>::Value4(T x, T y, T z, T w) : x{ x }, y{ y }, z{ z }, w{ w } {}
		template <typename T> Value4<T>::Value4(const Value3<T>& vector3, T w) : x{ vector3.x }, y{ vector3.y }, z{ vector3.z }, w{ w } {}

		template <typename T> Value4<T> Value4<T>::operator+(const Value4& r) const { return { x + r.x, y + r.y, z + r.z, w + r.w }; }
		template <typename T> Value4<T> Value4<T>::operator-(const Value4& r) const { return { x - r.x, y - r.y, z - r.z, w - r.w }; }
		template <typename T> Value4<T> Value4<T>::operator+(const T& r) const { return { x + r, y + r, z + r, w + r }; }
		template <typename T> Value4<T> Value4<T>::operator-(const T& r) const { return { x - r, y - r, z - r, w - r }; }
		template <typename T> Value4<T> Value4<T>::operator*(const T& r) const { return { x * r, y * r, z * r, w * r }; }
		template <typename T> Value4<T> Value4<T>::operator/(const T& r) const { return { x / r, y / r, z / r, w / r }; }
		template <typename T> void Value4<T>::operator+=(const Value4& r) { x += r.x; y += r.y; z += r.z; w += r.w; }
		template <typename T> void Value4<T>::operator-=(const Value4& r) { x -= r.x; y -= r.y; z -= r.z; w -= r.w; }
		template <typename T> void Value4<T>::operator+=(const T& r) { x += r; y += r; z += r; w += r; }
		template <typename T> void Value4<T>::operator-=(const T& r) { x -= r; y -= r; z -= r; w -= r; }
		template <typename T> void Value4<T>::operator*=(const T& r) { x *= r; y *= r; z *= r; w *= r; }
		template <typename T> void Value4<T>::operator/=(const T& r) { x /= r; y /= r; z /= r; w /= r; }
		template <typename T> Value4<T> Value4<T>::operator-() const { return{ -x, -y, -z, -w }; }

		template <typename T>
		T& Value4<T>::operator[](int idx)
		{
			return reinterpret_cast<T*>(&x)[idx];
		}

		template <typename T>
		T Value4<T>::operator[](int idx) const
		{
			return reinterpret_cast<const T*>(&x)[idx];
		}

		template <typename T> T Value4<T>::Get(int i) const { return *(&x + i); }
		template <typename T> void Value4<T>::Set(T f, int i) { *(&x + i) = f; }
		template <typename T> void Value4<T>::Inverse() { x = -x; y = -y; z = -z; w = -w; }
		template <typename T> void Value4<T>::Scale(const Value4& r) { x *= r.x; y *= r.y; z *= r.z; w *= r.w; }
		template <typename T> T Value4<T>::Length() const { return sqrtf(x * x + y * y + z * z + w * w); }
		template <typename T> T Value4<T>::LengthSq() const { return x * x + y * y + z * z + w * w; }
		template <typename T> T Value4<T>::Dot(const Value4& other) const { return { x * other.x + y * other.y + z * other.z + w * other.w }; }

		template <typename T>
		void Value4<T>::Normalize()
		{
			const T invLength{ static_cast<T>(1) / Length() };
			*this *= invLength;
		}

		template <typename T>
		Value4<T> Value4<T>::Normalized() const
		{
			const T invLength{ static_cast<T>(1) / Length() };
			return this * invLength;
		}
	}
}
