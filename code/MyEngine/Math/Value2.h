#pragma once
namespace MyEngine
{
	namespace Math
	{
		template <typename T>
		struct Value2
		{
			Value2();
			Value2(T x, T y);
			Value2(T both);
			~Value2() = default;

			Value2(const Value2& other) = default;
			Value2(Value2&& other) noexcept = default;
			Value2& operator=(const Value2& other) = default;
			Value2& operator=(Value2&& other) noexcept = default;
			
			Value2 operator+(const Value2& r) const;
			Value2 operator-(const Value2& r) const;
			Value2 operator+(const T& r) const;
			Value2 operator-(const T& r) const;
			Value2 operator*(const T& r) const;
			Value2 operator/(const T& r) const;
			void operator+=(const Value2& r);
			void operator-=(const Value2& r);
			void operator+=(const T& r);
			void operator-=(const T& r);
			void operator*=(const T& r);
			void operator/=(const T& r);

			T x, y;
		};

		template <typename T> Value2<T>::Value2() : x{ 0 }, y{ 0 } {}
		template <typename T> Value2<T>::Value2(T x, T y) : x{ x }, y{ y } {}
		template <typename T> Value2<T>::Value2(T both) : x{both}, y{both} {}
		template <typename T> Value2<T> Value2<T>::operator+(const Value2& r) const { return { x + r.x, y + r.y }; }
		template <typename T> Value2<T> Value2<T>::operator-(const Value2& r) const { return { x - r.x, y - r.y }; }
		template <typename T> Value2<T> Value2<T>::operator+(const T& r) const { return { x + r, y + r }; }
		template <typename T> Value2<T> Value2<T>::operator-(const T& r) const { return { x - r, y - r }; }
		template <typename T> Value2<T> Value2<T>::operator*(const T& r) const { return { x * r, y * r }; }
		template <typename T> Value2<T> Value2<T>::operator/(const T& r) const { return { x / r, y / r }; }
		template <typename T> void Value2<T>::operator+=(const Value2& r) { x += r.x; y += r.y; }
		template <typename T> void Value2<T>::operator-=(const Value2& r) { x -= r.x; y -= r.y; }
		template <typename T> void Value2<T>::operator+=(const T& r) { x += r; y += r; }
		template <typename T> void Value2<T>::operator-=(const T& r) { x -= r; y -= r; }
		template <typename T> void Value2<T>::operator*=(const T& r) { x *= r; y *= r; }
		template <typename T> void Value2<T>::operator/=(const T& r) { x /= r; y /= r; }

	}
}
