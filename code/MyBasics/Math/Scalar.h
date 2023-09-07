#pragma once

#undef max
#undef min
#include <cmath>

namespace MyEngine
{
	template<typename T>
	class Scalar
	{
	public:
		template<typename D> static T Unlerp(const D& value, const D& begin, const D& end);
		template<typename D> static D Lerp(const T& alpha, const D& begin, const D& end);
		template<typename D> static D LerpClamp(const T& alpha, const D& begin, const D& end);
		template<typename D> static T Cast(const D& value);
		template<typename D> static T Ceil(const D& value);
		template<typename D> static T Floor(const D& value);

		static int Round(const T& value);
		static T Clamped(const T& value, const T& min, const T& max);

		static T Max(const T& a, const T& b);
		static T Min(const T& a, const T& b);
		static T Max();
		static T Min();
		static void MinAndMax(const T& a, const T& b, T& min, T& max);
		static void SortAscending(T& a, T& b);

		static bool HasOverlap(float a1, float a2, float b1, float b2);
	};

	template <typename T>
	template <typename D>
	T Scalar<T>::Unlerp(const D& value, const D& begin, const D& end)
	{
		return static_cast<T>(value - begin) / static_cast<T>(end - begin);
	}

	template <typename T>
	template <typename D>
	D Scalar<T>::Lerp(const T& alpha, const D& begin, const D& end)
	{
		return begin + static_cast<D>(static_cast<T>(end - begin) * alpha);
	}

	template <typename T>
	template <typename D>
	D Scalar<T>::LerpClamp(const T& alpha, const D& begin, const D& end)
	{
		return Clamped(Lerp(alpha, begin, end), begin, end);
	}

	template <typename T>
	template <typename D>
	T Scalar<T>::Cast(const D& value)
	{
		return static_cast<T>(value);
	}

	template <typename T>
	template <typename D>
	T Scalar<T>::Ceil(const D& value)
	{
		return static_cast<T>(ceil(value));
	}

	template <typename T>
	template <typename D>
	T Scalar<T>::Floor(const D& value)
	{
		return static_cast<T>(floor(value));
	}

	template <typename T>
	int Scalar<T>::Round(const T& value)
	{
		return static_cast<int>(round(value));
	}

	template <typename T>
	T Scalar<T>::Clamped(const T& value, const T& min, const T& max)
	{
		if (value <= min) return min;
		if (value >= max) return max;
		return value;
	}

	template <typename T>
	T Scalar<T>::Max(const T& a, const T& b)
	{
		return a >= b ? a : b;
	}

	template <typename T>
	T Scalar<T>::Min(const T& a, const T& b)
	{
		return a <= b ? a : b;
	}

	template <typename T>
	T Scalar<T>::Max()
	{
		return std::numeric_limits<T>().max();
	}

	template <typename T>
	T Scalar<T>::Min()
	{
		return std::numeric_limits<T>().min();
	}

	template <typename T>
	void Scalar<T>::MinAndMax(const T& a, const T& b, T& min, T& max)
	{
		if (a <= b)
		{
			min = a;
			max = b;
		}
		else
		{
			min = b;
			max = a;
		}
	}

	template <typename T>
	void Scalar<T>::SortAscending(T& a, T& b)
	{
		if (a <= b) return;
		const float temp{ a };
		a = b;
		b = temp;
	}

	template <typename T>
	bool Scalar<T>::HasOverlap(float a1, float a2, float b1, float b2)
	{
		SortAscending(a1, a2);
		SortAscending(b1, b2);

		if (a1 <= b1)
			return a2 >= b1;
		return a1 <= b2;
	}
}
