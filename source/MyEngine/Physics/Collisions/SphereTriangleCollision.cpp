#include "pch.h"
#include "SphereTriangleCollision.h"

#include "LinePlaneCollision.h"
#include "LineSphereCollision.h"
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

/*
	Real-Time Collision Detection book
		5.5.6 Intersecting Moving Sphere Against Triangle (and Polygon)(p.226)
*/
bool SphereTriangleCollision::Detect(
	const Sphere& sphere, const Float3& direction, float amount,
	const Array<Float3>& points, const Array<Float3>& normals)
{
	for (unsigned iPoint{ 2 }, iNormal{ 0 };
		iPoint < points.GetSize();
		iPoint += 3, iNormal++)
	{
		const Float3* pTriangle{ &points[iPoint - 2] };
		const Float3& normal{ normals[iNormal] };

		const Float3 d{ sphere.GetCenter() - normal * sphere.GetRadius() }; //point on sphere that will hit plane first
		Float3 p{}; //point on plane that will be hit first

		if (!LinePlaneCollision::Detect(
			d, direction, amount, points[iPoint], normal, p))
			continue; //todo: only false if parallel 
							//but this could still be a hit if sphere starts in plane

		Float3 q{}; //closest triangle point to p
		if (PointTriangleCollision::DetectOrClosest(p, pTriangle, q))
			return true;

		Ray returnRay{ q, -direction, amount };
		if (LineSphereCollision::Detect(returnRay, sphere))
			return true;
	}
	return false;
}
