#include "pch.h"
#include "Triangle.h"

using namespace MyEngine;

Float3 Triangle::FindNormal() const
{
	return FindNormal(Points);
}

Float3 Triangle::FindNormal(const Float3& p0, const Float3& p1, const Float3& p2)
{
	const Float3 to1{ p1 - p0 };
	const Float3 to2{ p2 - p0 };
	return to1.Cross(to2).NormalizedSafe();
}

Float3 Triangle::FindNormal(const Float3* pPoint)
{
	const Float3 to1{ pPoint[1] - pPoint[0]};
	const Float3 to2{ pPoint[2] - pPoint[0] };
	return to1.Cross(to2).NormalizedSafe();
}
