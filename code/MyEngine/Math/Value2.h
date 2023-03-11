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

			T Get(int i) const;
			void Set(T v, int i);

			void Reverse();
			void Scale(const Value2<T>& r);
			T Length() const;
			void Normalize();
			Value2<T> Normalized() const;

			T x, y;
		};

		template <typename T> Value2<T>::Value2() : x{ 0 }, y{ 0 } {}
		template <typename T> Value2<T>::Value2(T x, T y) : x{ x }, y{ y } {}
		template <typename T> Value2<T>::Value2(T both) : x{ both }, y{ both } {}
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

		template <typename T>
		T Value2<T>::Get(int i) const
		{
			if (i == 0)return x;
			return y;
		}

		template <typename T>
		void Value2<T>::Set(T v, int i)
		{
			if (i == 0)x = v;
			y = v;
		}

		template <typename T>
		void Value2<T>::Reverse()
		{
			x = -x;
			y = -y;
		}

		template <typename T>
		void Value2<T>::Scale(const Value2<T>& r)
		{
			x *= r.x;
			y *= r.y;
		}

		template <typename T>
		T Value2<T>::Length() const
		{
			return sqrt(x * x + y * y);
		}

		template <typename T>
		void Value2<T>::Normalize()
		{
			const T scale{ static_cast<T>(1) / Length() };
			x *= scale;
			y *= scale;
		}

		template <typename T>
		Value2<T> Value2<T>::Normalized() const
		{
			const T scale{ static_cast<T>(1) / Length() };
			return{ x * scale, y * scale };
		}
	}
}
