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
	const Sphere& sphere, const List<Float3>& points)
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
	const Sphere& sphere, const List<Float3>& points)
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

bool SphereTriangleCollision::Detect(const Sphere& sphere, const Float3* pPoints)
{
	const Float3 closest{ PointTriangleCollision::Closest(
		sphere.GetCenter(),
		pPoints)
	};

	const Float3 v{ closest - sphere.GetCenter() };
	if (v.LengthSq() <= sphere.GetRadiusSq())
		return true;
	return false;
}

/*
	Real-Time Collision Detection book
		5.5.6 Intersecting Moving Sphere Against Triangle (and Polygon)(p.226)

	Need some improvement. For example if the sphere starts inside the triangle.
*/
bool SphereTriangleCollision::Detect(
	const Sphere& sphere, const Float3& direction, float length,
	PtrRangeConst<Float3> points, PtrRangeConst<Float3> normals)
{
	float closestT{ std::nextafterf(length, Float::MAX) };
	unsigned closestTriangle{ Uint::MAX };

	for (unsigned iPoint{ 2 }, iTriangle{ 0 };
		iPoint < points.count;
		iPoint += 3, iTriangle++)
	{
		const Float3* pTriangle{ &points[iPoint - 2] };
		const Float3& normal{ normals[iTriangle] };

		if (SphereTriangleCollision::Detect(sphere, pTriangle))
		{
			closestT = 0;
			closestTriangle = iTriangle;
			return true;
		}

		const Float3 d{ sphere.GetCenter() - normal * sphere.GetRadius() }; //point on sphere that will hit plane first

		float t;
		LinePlaneCollision::Detect(
			d, direction, points[iPoint], normal, t);

		if (t < 0 || t >= closestT)
			continue;

		const Float3 p{ d + direction * t }; //point on plane that will be hit first

		Float3 q{}; //closest triangle point to p
		if (PointTriangleCollision::DetectOrClosest(p, pTriangle, q))
		{
			closestT = t;
			closestTriangle = iTriangle;
			continue;
		}

		Ray returnRay{ q, -direction, closestT };
		if (LineSphereCollision::Detect(returnRay, sphere, t))
		{
			closestT = t;
			closestTriangle = iTriangle;
		}
	}

	if (closestTriangle == Uint::MAX)
		return false;
	return true;
}
