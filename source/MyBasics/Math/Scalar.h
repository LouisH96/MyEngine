#pragma once

#undef max
#undef min
#include <cmath>
#include <limits>

namespace MyEngine
{
	template<typename T>
	class Scalar
	{
	public:
		static constexpr T MIN{ std::numeric_limits<T>().min() };
		static constexpr T MAX{ std::numeric_limits<T>().max() };
		static constexpr T INVALID{ MAX };

		static constexpr bool HasDecimals();

		template<typename D> static T Unlerp(const D& value, const D& begin, const D& end);
		template<typename D> static D Lerp(const T& alpha, const D& begin, const D& end);
		template<typename D> static D LerpClamp(const T& alpha, const D& begin, const D& end);
		template<typename D> static T Cast(const D& value);
		template<typename D> static T Ceil(const D& value);
		template<typename D> static T Floor(const D& value);
		template<typename D> static T Round(const D& value);

		static T Clamped(const T& value, const T& min, const T& max);
		static T TriangleNumber(const T& value);
		static T Sign(const T& value);

		static T Max(const T& a, const T& b);
		static T Max(const T& a, const T& b, const T& c);
		static T Min(const T& a, const T& b);
		static T Min(const T& a, const T& b, const T& c);
		static void UpdateMax(T& currentMax, const T& possibleMax);
		static void UpdateMin(T& currentMin, const T& possibleMin);

		static T Max();
		static T Min();
		static void MinAndMax(const T& a, const T& b, T& min, T& max);
		static void SortAscending(T& a, T& b);

		static bool HasOverlap(T a1, T a2,T b1, T b2);
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
	template <typename D>
	T Scalar<T>::Round(const D& value)
	{
		return static_cast<T>( round(value) );
	}

	template <typename T>
	T Scalar<T>::Clamped(const T& value, const T& min, const T& max)
	{
		if (value <= min) return min;
		if (value >= max) return max;
		return value;
	}

	template <typename T>
	T Scalar<T>::TriangleNumber(const T& value)
	{
		return static_cast<T>(value * (value + static_cast<T>(1)) * (value + static_cast<T>(2)) / static_cast<T>(6));
	}

	template <typename T>
	T Scalar<T>::Sign(const T& value)
	{
		return static_cast<T>(T{ 0 } < value) - (value < T{ 0 });
	}

	template <typename T>
	T Scalar<T>::Max(const T& a, const T& b)
	{
		return a >= b ? a : b;
	}

	template <typename T>
	T Scalar<T>::Max(const T& a, const T& b, const T& c)
	{
		if (a >= b)
			return a >= c ? a : c;
		else
			return b >= c ? b : c;
	}

	template <typename T>
	T Scalar<T>::Min(const T& a, const T& b)
	{
		return a <= b ? a : b;
	}

	template <typename T>
	T Scalar<T>::Min(const T& a, const T& b, const T& c)
	{
		if (a <= b)
			return a <= c ? a : c;
		else
			return b <= c ? b : c;
	}

	template<typename T>
	inline void Scalar<T>::UpdateMax(T& currentMax, const T& possibleMax)
	{
		if (possibleMax > currentMax)
			currentMax = possibleMax;
	}

	template<typename T>
	inline void Scalar<T>::UpdateMin(T& currentMin, const T& possibleMin)
	{
		if (possibleMin < currentMin)
			currentMin = possibleMin;
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
	bool Scalar<T>::HasOverlap(T a1, T a2, T b1, T b2)
	{
		SortAscending(a1, a2);
		SortAscending(b1, b2);

		if (a1 <= b1)
			return a2 >= b1;
		return a1 <= b2;
	}
	template<typename T>
	inline constexpr bool Scalar<T>::HasDecimals()
	{
		return static_cast<T>(.5) != 0;
	}
}
