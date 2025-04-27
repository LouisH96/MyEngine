#include "pch.h"
#include "Triangle.h"

using namespace MyEngine;

Float3 Triangle::FindNormal() const
{
	return FindNormal(Points);
}

bool Triangle::FindNormal(Float3& normal) const
{
	return Triangle::FindNormal(Points, normal);
}

Float3 Triangle::FindNormal(const Float3& p0, const Float3& p1, const Float3& p2)
{
	const Float3 to1{ p1 - p0 };
	const Float3 to2{ p2 - p0 };
	return to1.Cross(to2).NormalizedSafe();
}

Float3 Triangle::FindNormal(const Float3* pPoint)
{
	const Float3 to1{ pPoint[1] - pPoint[0] };
	const Float3 to2{ pPoint[2] - pPoint[0] };
	return to1.Cross(to2).NormalizedSafe();
}

bool Triangle::FindNormal(const Float3& p0, const Float3& p1, const Float3& p2, Float3& normal)
{
	const Float3 to1{ p1 - p0 };
	const Float3 to2{ p2 - p0 };
	const Float3 cross{ to1.Cross(to2) };
	const float sqLength{ cross.LengthSq() };
	if (sqLength == 0)
		return false;
	normal = cross / sqrtf(sqLength);
	return true;
}

bool Triangle::FindNormal(const Float3* pPoints, Float3& normal)
{
	return FindNormal(pPoints[0], pPoints[1], pPoints[2], normal);
}
