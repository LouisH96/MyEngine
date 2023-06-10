#pragma once

namespace MyEngine
{
	namespace Math
	{
		template<typename T>
		class ScalarMath
		{
		public:
			template<typename D> static T Unlerp(const D& value, const D& begin, const D& end);
			template<typename D> static D Lerp(const T& alpha, const D& begin, const D& end);
			template<typename D> static D LerpClamp(const T& alpha, const D& begin, const D& end);

			static T Clamp(const T& value, const T& min, const T& max);
			static T Max();
			static T Min();
		};

		template <typename T>
		template <typename D>
		T ScalarMath<T>::Unlerp(const D& value, const D& begin, const D& end)
		{
			return static_cast<T>(value - begin) / static_cast<T>(end - begin);
		}

		template <typename T>
		template <typename D>
		D ScalarMath<T>::Lerp(const T& alpha, const D& begin, const D& end)
		{
			return begin + static_cast<D>(static_cast<T>(end - begin) * alpha);
		}

		template <typename T>
		template <typename D>
		D ScalarMath<T>::LerpClamp(const T& alpha, const D& begin, const D& end)
		{
			return Clamp(Lerp(alpha, begin, end), begin, end);
		}

		template <typename T>
		T ScalarMath<T>::Clamp(const T& value, const T& min, const T& max)
		{
			if (value <= min) return min;
			if (value >= max) return max;
			return value;
		}

		template <typename T>
		T ScalarMath<T>::Max()
		{
			return std::numeric_limits<T>().max();
		}

		template <typename T>
		T ScalarMath<T>::Min()
		{
			return std::numeric_limits<T>().min();
		}

		using DoubleMath = ScalarMath<double>;
		using FloatMath = ScalarMath<float>;
	}
}
