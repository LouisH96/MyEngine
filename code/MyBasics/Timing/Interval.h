#pragma once
#include "Time.h"

namespace MyEngine
{
	class Interval
	{
	public:
		Interval();
		explicit Interval(const App::Time::Duration& interval, bool startNow);

		bool IsPassed();

	private:
		App::Time::Duration m_Interval;
		App::Time::TimePoint m_Next;
	};
}
