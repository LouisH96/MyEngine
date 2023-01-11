#pragma once
#include <chrono>

#include "Time.h"

namespace MyEngine
{
	namespace Wrappers
	{
		class FpsControl
		{
		public:
			FpsControl(const FpsControl& other) = delete;
			FpsControl(FpsControl&& other) noexcept = delete;
			FpsControl& operator=(const FpsControl& other) = delete;
			FpsControl& operator=(FpsControl&& other) noexcept = delete;

			FpsControl(float desiredFps);
			~FpsControl() = default;

			void Wait(); //Frame should be build after this wait
			uint16_t GetNrFramesLastSec() const { return m_NrFramesLastSec; }
			float GetDurationLastFrame() const { return m_DurationLastFrameSec; }

		private:
			Time::Duration m_DesiredInterval;
			float m_DesiredIntervalSec{};

			Time::TimePoint m_DesiredNextFrame;
			Time::TimePoint m_EndCurrentSecond;
			
			uint16_t m_NrFramesThisSec{ 0 };
			uint16_t m_NrFramesLastSec{ 0 };

			float m_DurationLastFrameSec{};
		};
	}
}
