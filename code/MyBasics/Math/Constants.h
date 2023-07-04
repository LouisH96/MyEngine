#pragma once
#include <limits>

namespace MyEngine
{
	namespace Math
	{
		class Constants
		{
		public:
			static constexpr float PI = 3.14159265358979323846f;
			static constexpr float PI2 = 3.14159265358979323846f * 2;
			static constexpr float PI_DIV2 = 1.57079632679489661923f;
			static constexpr float TO_RAD = PI / 180.f;
			static constexpr float TO_DEG = 180.f / PI;
			static constexpr float SQR2 = 1.4142135623730950488016887242097f;
			static constexpr float DIVSQR2 = 1.f / SQR2;

			static constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
			static constexpr float FLOAT_INVALID = std::numeric_limits<float>::infinity();
		};
	}
}
