#pragma once
namespace MyEngine
{
	namespace Math
	{
		template <typename T>
		struct Vector2
		{
			Vector2();
			Vector2(T x, T y);
			Vector2(T both);
			~Vector2() = default;

			Vector2(const Vector2& other) = default;
			Vector2(Vector2&& other) noexcept = default;
			Vector2& operator=(const Vector2& other) = default;
			Vector2& operator=(Vector2&& other) noexcept = default;

			template<typename O>
			Vector2(const Vector2<O>& other);
			template<typename O>
			Vector2(Vector2<O>&& other) noexcept;
			template<typename O>
			Vector2& operator=(const Vector2<O>& other);
			template<typename O>
			Vector2& operator=(Vector2<O>&& other) noexcept;

			Vector2 operator+(const Vector2& r) const;
			Vector2 operator-(const Vector2& r) const;
			Vector2 operator+(const T& r) const;
			Vector2 operator-(const T& r) const;
			Vector2 operator*(const T& r) const;
			Vector2 operator/(const T& r) const;
			void operator+=(const Vector2& r);
			void operator-=(const Vector2& r);
			void operator+=(const T& r);
			void operator-=(const T& r);
			void operator*=(const T& r);
			void operator/=(const T& r);
			Vector2 operator-() const;

			T Get(int i) const;
			void Set(T v, int i);

			void Reverse();
			void Scale(const Vector2<T>& r);
			Vector2 Scaled(const Vector2& scale) const;
			void Divide(const Vector2& divisor);
			Vector2 Divided(const Vector2& divisor) const;
			T Length() const;
			T LengthSq() const;
			void Normalize();
			Vector2<T> Normalized() const;
			float Cross(const Vector2<T>& o);

			bool IsLeftAbove(const Vector2& comparedTo) const;
			bool IsRightAbove(const Vector2& comparedTo) const;
			bool IsLeftBelow(const Vector2& comparedTo) const;
			bool IsRightBelow(const Vector2& comparedTo) const;

			T x, y;
		};

		template <typename T> Vector2<T>::Vector2() : x{ 0 }, y{ 0 } {}
		template <typename T> Vector2<T>::Vector2(T x, T y) : x{ x }, y{ y } {}
		template <typename T> Vector2<T>::Vector2(T both) : x{ both }, y{ both } {}

		template <typename T> template <typename O>
		Vector2<T>::Vector2(const Vector2<O>& other) : x{static_cast<T>(other.x)} , y{static_cast<T>(other.y)} {}
		template <typename T> template <typename O>
		Vector2<T>::Vector2(Vector2<O>&& other) noexcept : x{ static_cast<T>(other.x) }, y{ static_cast<T>(other.y) } {}

		template <typename T>
		template <typename O>
		Vector2<T>& Vector2<T>::operator=(const Vector2<O>& other)
		{
			x = static_cast<T>(other.x);
			y = static_cast<T>(other.y);
			return *this;
		}

		template <typename T>
		template <typename O>
		Vector2<T>& Vector2<T>::operator=(Vector2<O>&& other) noexcept
		{
			x = static_cast<T>(other.x);
			y = static_cast<T>(other.y);
			return *this;
		}

		template <typename T> Vector2<T> Vector2<T>::operator+(const Vector2& r) const { return { x + r.x, y + r.y }; }
		template <typename T> Vector2<T> Vector2<T>::operator-(const Vector2& r) const { return { x - r.x, y - r.y }; }
		template <typename T> Vector2<T> Vector2<T>::operator+(const T& r) const { return { x + r, y + r }; }
		template <typename T> Vector2<T> Vector2<T>::operator-(const T& r) const { return { x - r, y - r }; }
		template <typename T> Vector2<T> Vector2<T>::operator*(const T& r) const { return { x * r, y * r }; }
		template <typename T> Vector2<T> Vector2<T>::operator/(const T& r) const { return { x / r, y / r }; }
		template <typename T> void Vector2<T>::operator+=(const Vector2& r) { x += r.x; y += r.y; }
		template <typename T> void Vector2<T>::operator-=(const Vector2& r) { x -= r.x; y -= r.y; }
		template <typename T> void Vector2<T>::operator+=(const T& r) { x += r; y += r; }
		template <typename T> void Vector2<T>::operator-=(const T& r) { x -= r; y -= r; }
		template <typename T> void Vector2<T>::operator*=(const T& r) { x *= r; y *= r; }
		template <typename T> void Vector2<T>::operator/=(const T& r) { x /= r; y /= r; }

		template <typename T>
		Vector2<T> Vector2<T>::operator-() const
		{
			return{ -x, -y };
		}

		template <typename T>
		T Vector2<T>::Get(int i) const
		{
			if (i == 0)return x;
			return y;
		}

		template <typename T>
		void Vector2<T>::Set(T v, int i)
		{
			if (i == 0)x = v;
			y = v;
		}

		template <typename T>
		void Vector2<T>::Reverse()
		{
			x = -x;
			y = -y;
		}

		template <typename T>
		void Vector2<T>::Scale(const Vector2<T>& r)
		{
			x *= r.x;
			y *= r.y;
		}

		template <typename T>
		Vector2<T> Vector2<T>::Scaled(const Vector2& scale) const
		{
			return { x * scale.x, y * scale.y };
		}

		template <typename T>
		void Vector2<T>::Divide(const Vector2& divisor)
		{
			x /= divisor.x;
			y /= divisor.y;
		}

		template <typename T>
		Vector2<T> Vector2<T>::Divided(const Vector2& divisor) const
		{
			return { x / divisor.x, y / divisor.y };
		}

		template <typename T>
		T Vector2<T>::Length() const
		{
			return sqrt(x * x + y * y);
		}

		template <typename T>
		T Vector2<T>::LengthSq() const
		{
			return x * x + y * y;
		}

		template <typename T>
		void Vector2<T>::Normalize()
		{
			const T scale{ static_cast<T>(1) / Length() };
			x *= scale;
			y *= scale;
		}

		template <typename T>
		Vector2<T> Vector2<T>::Normalized() const
		{
			const T scale{ static_cast<T>(1) / Length() };
			return{ x * scale, y * scale };
		}

		template <typename T>
		float Vector2<T>::Cross(const Vector2<T>& o)
		{
			return (x * o.y) - (y * o.x);
		}

		template <typename T>
		bool Vector2<T>::IsLeftAbove(const Vector2& comparedTo) const
		{
			return x < comparedTo.x && y > comparedTo.y;
		}

		template <typename T>
		bool Vector2<T>::IsRightAbove(const Vector2& comparedTo) const
		{
			return x > comparedTo.x && y > comparedTo.y;
		}

		template <typename T>
		bool Vector2<T>::IsRightBelow(const Vector2& comparedTo) const
		{
			return x > comparedTo.x && y < comparedTo.y;
		}

		template <typename T>
		bool Vector2<T>::IsLeftBelow(const Vector2& comparedTo) const
		{
			return x < comparedTo.x && y < comparedTo.y;
		}
	}
}
