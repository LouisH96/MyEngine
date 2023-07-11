#pragma once
#include <chrono>

#include "Timing/Time.h"

namespace MyEngine
{
	namespace Rendering
	{
		class FpsDisplay;
	}
}

namespace MyEngine
{
	namespace App
	{
		class FpsControl
		{
		public:
			FpsControl(const FpsControl& other) = delete;
			FpsControl(FpsControl&& other) noexcept = delete;
			FpsControl& operator=(const FpsControl& other) = delete;
			FpsControl& operator=(FpsControl&& other) noexcept = delete;

			FpsControl(float desiredFps, Rendering::FpsDisplay* pDisplay = nullptr);
			~FpsControl() = default;

			void Wait(); //Frame should be build after this wait
			void NoWait();
			float GetDurationLastFrame() const { return m_DurationLastFrame; }
			uint16_t GetNrFramesPrevSec() const { return m_NrFramesPrevSec; }
			void SetFpsDisplay(Rendering::FpsDisplay& display);

		private:
			Rendering::FpsDisplay* m_pDisplay;
			Time::Duration m_DesiredInterval;

			Time::TimePoint m_BeginPrevFrame;
			Time::TimePoint m_BeginPrevUpdate;

			Time::TimePoint m_EndCurrentSecond;
			uint16_t m_NrFramesThisSec{ 0 };
			uint16_t m_NrFramesPrevSec{};
			float m_DurationLastFrame;
		};
	}
}
