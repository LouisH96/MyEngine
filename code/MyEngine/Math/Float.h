#pragma once

namespace MyEngine
{
	namespace Math
	{
		class Float
		{
		public:
			template<typename T> static float Unlerp(const T& value, const T& begin, const T& end);
			template<typename T> static T Lerp(float alpha, const T& begin, const T& end);
		};

		template <typename T>
		float Float::Unlerp(const T& value, const T& begin, const T& end)
		{
			return static_cast<float>(value - begin) / static_cast<float>(end - begin);
		}

		template <typename T>
		T Float::Lerp(float alpha, const T& begin, const T& end)
		{
			return begin + static_cast<float>(end - begin) * alpha;
		}
	}
}
