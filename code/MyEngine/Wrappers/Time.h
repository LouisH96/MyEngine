#pragma once
#include <chrono>

namespace MyEngine
{
	namespace Wrappers
	{
		class Time
		{
		public:
			using Clock = std::chrono::high_resolution_clock;
			using TimePoint = Clock::time_point;
			using Duration = Clock::duration;
			static constexpr float SEC_TO_CLOCK_UNIT = static_cast<float>(Duration::period::den); //multiply seconds by this to be in the clock's unit
			static constexpr float CLOCK_UNIT_TO_SEC = 1.f / static_cast<float>(Duration::period::den); //multiply the clock's unit by this to be in seconds
			static constexpr Duration SEC_DURATION{ Duration::period::den };
		};
	}
}

