#pragma once
#include <string>
#include <Timing/Time.h>

namespace MyEngine
{
	namespace Timing
	{
		class Counter
		{
		public:
			Counter();

			void Begin();
			void End(const std::string& name) const;

		private:
			App::Time::TimePoint m_Begin{};
		};
	}
}
