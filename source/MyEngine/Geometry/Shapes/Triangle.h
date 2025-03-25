#pragma once
#include <Math\Vectors.h>

namespace MyEngine
{
class Triangle
{
public:
	static constexpr unsigned NR_POINTS{ 3 };
	
	Float3 Points[NR_POINTS];

	Float3 FindNormal() const;

	//lh-coord, thus: clockwise is looking at front.
	static Float3 FindNormal(const Float3& p0, const Float3& p1, const Float3& p2);
	static Float3 FindNormal(const Float3* pPoint);
};
}

