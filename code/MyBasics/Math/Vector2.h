#pragma once
#include <limits>

#include <DataStructures\PtrRangeConst.h>

#undef max
#undef min

namespace MyEngine
{
	template <typename T>
	struct Vector2
	{
		using Unit = T;

		Vector2();
		Vector2(T x, T y);
		explicit Vector2(T both);
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

		bool operator==(const Vector2& other);

		Vector2 operator+(const Vector2& r) const;
		Vector2 operator-(const Vector2& r) const;
		Vector2 operator*(const Vector2& r) const;
		Vector2 operator/(const Vector2& r) const;
		Vector2 operator+(const T& r) const;
		Vector2 operator-(const T& r) const;
		Vector2 operator*(const T& r) const;
		Vector2 operator/(const T& r) const;
		void operator+=(const Vector2& r);
		void operator-=(const Vector2& r);
		void operator*=(const Vector2& r);
		void operator/=(const Vector2& r);
		void operator+=(const T& r);
		void operator-=(const T& r);
		void operator*=(const T& r);
		void operator/=(const T& r);
		Vector2 operator-() const;

		T Get(int i) const;
		void Set(T v, int i);

		void Reverse();
		T Length() const;
		T LengthSq() const;
		void Normalize();
		Vector2<T> Normalized() const;
		float Cross(const Vector2& o) const;
		float Dot(const Vector2& o) const;
		Vector2<int> Floored() const;
		Vector2<int> Ceiled() const;
		T Distance(const Vector2& other) const;
		T DistanceSq(const Vector2& other) const;
		T Smallest() const;
		Vector2<T> Abs() const;

		bool IsLeftAbove(const Vector2& comparedTo) const;
		bool IsRightAbove(const Vector2& comparedTo) const;
		bool IsLeftBelow(const Vector2& comparedTo) const;
		bool IsRightBelow(const Vector2& comparedTo) const;

		static Vector2 GetSize(const Vector2* pData, unsigned count);

		template<typename PtrRange>
		static void GetBounds(PtrRange data, Vector2& min, Vector2& max);

		bool IsZero() const;

		T x, y;
	};

	template <typename T> Vector2<T>::Vector2() : x{ 0 }, y{ 0 } {}
	template <typename T> Vector2<T>::Vector2(T x, T y) : x{ x }, y{ y } {}
	template <typename T> Vector2<T>::Vector2(T both) : x{ both }, y{ both } {}

	template <typename T> template <typename O>
	Vector2<T>::Vector2(const Vector2<O>& other) : x{ static_cast<T>(other.x) }, y{ static_cast<T>(other.y) } {}
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

	template <typename T>
	bool Vector2<T>::operator==(const Vector2& other)
	{
		return x == other.x && y == other.y;
	}

	template <typename T> Vector2<T> Vector2<T>::operator+(const Vector2& r) const { return { static_cast<T>(x + r.x), static_cast<T>(y + r.y) }; }
	template <typename T> Vector2<T> Vector2<T>::operator-(const Vector2& r) const { return { static_cast<T>(x - r.x), static_cast<T>(y - r.y) }; }

	template <typename T>
	Vector2<T> Vector2<T>::operator*(const Vector2& r) const
	{
		return { x * r.x, y * r.y };
	}

	template <typename T>
	Vector2<T> Vector2<T>::operator/(const Vector2& r) const
	{
		return { x / r.x, y / r.y };
	}

	template <typename T> Vector2<T> Vector2<T>::operator+(const T& r) const { return { static_cast<T>(x + r), static_cast<T>(y + r) }; }
	template <typename T> Vector2<T> Vector2<T>::operator-(const T& r) const { return { static_cast<T>(x - r), static_cast<T>(y - r) }; }
	template <typename T> Vector2<T> Vector2<T>::operator*(const T& r) const { return { static_cast<T>(x * r), static_cast<T>(y * r) }; }
	template <typename T> Vector2<T> Vector2<T>::operator/(const T& r) const { return { static_cast<T>(x / r), static_cast<T>(y / r) }; }
	template <typename T> void Vector2<T>::operator+=(const Vector2& r) { x += r.x; y += r.y; }
	template <typename T> void Vector2<T>::operator-=(const Vector2& r) { x -= r.x; y -= r.y; }

	template <typename T>
	void Vector2<T>::operator*=(const Vector2& r)
	{
		x *= r.x;
		y *= r.y;
	}

	template <typename T>
	void Vector2<T>::operator/=(const Vector2& r)
	{
		x /= r.x;
		y /= r.y;
	}

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
	float Vector2<T>::Cross(const Vector2& o) const
	{
		return (x * o.y) - (y * o.x);
	}

	template <typename T>
	float Vector2<T>::Dot(const Vector2& o) const
	{
		return x * o.x + y * o.y;
	}

	template <typename T>
	Vector2<int> Vector2<T>::Floored() const
	{
		return { static_cast<int>(floor(x)), static_cast<int>(floor(y)) };
	}

	template <typename T>
	Vector2<int> Vector2<T>::Ceiled() const
	{
		return { static_cast<int>(ceil(x)), static_cast<int>(ceil(y)) };
	}

	template <typename T>
	T Vector2<T>::Distance(const Vector2& other) const
	{
		float xDistance{ other.x - x };
		xDistance *= xDistance;
		float yDistance{ other.y - y };
		yDistance *= yDistance;
		return sqrt(xDistance + yDistance);
	}

	template <typename T>
	T Vector2<T>::DistanceSq(const Vector2& other) const
	{
		float xDistance{ other.x - x };
		xDistance *= xDistance;
		float yDistance{ other.y - y };
		yDistance *= yDistance;
		return  xDistance + yDistance;
	}

	template <typename T>
	T Vector2<T>::Smallest() const
	{
		return std::min(x, y);
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::Abs() const
	{
		return{ abs(x), abs(y) };
	}

	template <typename T>
	bool Vector2<T>::IsLeftAbove(const Vector2& comparedTo) const
	{
		return x < comparedTo.x&& y > comparedTo.y;
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
	Vector2<T> Vector2<T>::GetSize(const Vector2* pData, unsigned count)
	{
		Vector2 min, max;
		GetBounds(pData, count, min, max);
		return max - min;
	}

	template <typename T>
	template <typename PtrRange>
	void Vector2<T>::GetBounds(PtrRange data, Vector2& min, Vector2& max)
	{
		min = Vector2{ std::numeric_limits<T>::max() };
		max = Vector2{ std::numeric_limits<T>::lowest() };
		for (unsigned i = 0; i < data.count; i++)
		{
			const Vector2& element{ data[i] };
			if (element.x < min.x) min.x = element.x;
			if (element.y < min.y) min.y = element.y;
			if (element.x > max.x) max.x = element.x;
			if (element.y > max.y) max.y = element.y;
		}
	}

	template <typename T>
	bool Vector2<T>::IsZero() const
	{
		return x == T{ 0 } && y == T{ 0 };
	}

	template <typename T>
	bool Vector2<T>::IsLeftBelow(const Vector2& comparedTo) const
	{
		return x < comparedTo.x&& y < comparedTo.y;
	}
}
