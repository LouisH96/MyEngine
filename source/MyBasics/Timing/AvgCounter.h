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

