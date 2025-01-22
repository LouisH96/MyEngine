#pragma once

namespace MyEngine
{
class Triangle
{
public:
	static constexpr unsigned NR_POINTS{ 3 };

	//lh-coord, thus: clockwise is looking at top.
	static Float3 FindNormal(const Float3& p0, const Float3& p1, const Float3& p2);
};
}

