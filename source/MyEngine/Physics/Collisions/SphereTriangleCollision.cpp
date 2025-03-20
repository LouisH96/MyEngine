#include "pch.h"
#include "SphereTriangleCollision.h"

#include "PointTriangleCollision.h"

using namespace Physics;

bool SphereTriangleCollision::Detect_Rough(
	const Sphere& sphere, const Float3* pPoint)
{
	const float radiusSq{ sphere.GetRadiusSq() };

	for (unsigned iPoint{ 0 }; iPoint < Triangle::NR_POINTS; ++iPoint)
	{
		const Float3& point{ *(pPoint + iPoint) };
		if (point.DistanceSq(sphere.GetCenter()) <= radiusSq)
			return true;
	}
	return false;
}

bool SphereTriangleCollision::Detect_Rough(
	const Sphere& sphere, const Array<Float3>& points)
{
	const float radiusSq{ sphere.GetRadiusSq() };

	for (unsigned iPoint{ 0 }; iPoint < points.GetSize(); ++iPoint)
	{
		const Float3& point{ points[iPoint] };
		if (point.DistanceSq(sphere.GetCenter()) <= radiusSq)
			return true;
	}
	return false;
}

bool SphereTriangleCollision::Detect(
	const Sphere& sphere, const Array<Float3>& points)
{
	const float radiusSq{ sphere.GetRadiusSq() };
	for (unsigned iPoint{ 2 }; iPoint < points.GetSize(); iPoint += 3)
	{
		const Float3 closest{ PointTriangleCollision::Closest(
			sphere.GetCenter(),
			&points[iPoint - 2])
		};

		const Float3 v{ closest - sphere.GetCenter() };
		if (v.LengthSq() <= radiusSq)
			return true;
	}
	return false;
}
