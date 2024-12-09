#pragma once
#include <string>
#include <Timing/Time.h>

namespace MyEngine
{
namespace Timing
{
class AvgCounter
{
public:
	AvgCounter(std::string name, unsigned interval);

	void Begin();
	void End();

	bool IsInFirstHalf() const { return m_Count < (m_Interval / 2); }

private:
	std::string m_Name;
	App::Time::TimePoint m_Prev{};
	unsigned m_Interval;
	double m_Scale;

	unsigned m_Count;
	double m_Avg;

	void Print() const;
	void Reset();
};
}
}

