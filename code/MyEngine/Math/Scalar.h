#pragma once

#undef max
#undef min

namespace MyEngine
{
	namespace Math
	{
		template<typename T>
		class Scalar
		{
		public:
			template<typename D> static T Unlerp(const D& value, const D& begin, const D& end);
			template<typename D> static D Lerp(const T& alpha, const D& begin, const D& end);
			template<typename D> static D LerpClamp(const T& alpha, const D& begin, const D& end);
			template<typename D> static T Cast(const D& value);

			static int Ceil(const T& value);
			static int Floor(const T& value);
			static int Round(const T& value);
			static T Clamp(const T& value, const T& min, const T& max);
			static T Max();
			static T Min();
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
			return Clamp(Lerp(alpha, begin, end), begin, end);
		}

		template <typename T>
		template <typename D>
		T Scalar<T>::Cast(const D& value)
		{
			return static_cast<T>(value);
		}

		template <typename T>
		int Scalar<T>::Ceil(const T& value)
		{
			return ceil(value);
		}

		template <typename T>
		int Scalar<T>::Floor(const T& value)
		{
			return floor(value);
		}

		template <typename T>
		int Scalar<T>::Round(const T& value)
		{
			return static_cast<int>(round(value));
		}

		template <typename T>
		T Scalar<T>::Clamp(const T& value, const T& min, const T& max)
		{
			if (value <= min) return min;
			if (value >= max) return max;
			return value;
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

		using DoubleMath = Scalar<double>;
		using FloatMath = Scalar<float>;
	}
}
