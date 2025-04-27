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

float SphereTriangleCollision::MoveOut(
	const Sphere& sphere,
	const Float3& direction,
	PtrRangeConst<Float3> points,
	PtrRangeConst<Float3> triangleNormals)
{
	float t{ 0 };
	Sphere localSphere{ sphere };
	unsigned iLast{ 0 };
	unsigned nextILast{ triangleNormals.count };
	Float3 closest;

	while (iLast != nextILast)
	{
		iLast = nextILast;
		for (
			unsigned iPoint{ 2 }, iTriangle{ 0 };
			iTriangle < iLast;
			iPoint += 3, ++iTriangle)
		{
			const Float3* pPoint{ &points[iPoint - 2] };

			if (!Detect(localSphere, pPoint, closest))
				continue;

			if (closest.DistanceSq(localSphere.GetCenter()) + Constants::PHYSICS_EPSILON > localSphere.GetRadiusSq())
				continue;

			const float dot{
				direction.Dot(localSphere.GetCenter() - closest)
			};
			const Float3 perpendicular{
				closest + direction * dot
			};

			const float centerToEdge{
				sqrtf(localSphere.GetRadiusSq() - perpendicular.DistanceSq(localSphere.GetCenter()))
			};
			const float distance{
				centerToEdge - dot
			};

			t += distance + Constants::PHYSICS_EPSILON;
			localSphere.SetCenter(sphere.GetCenter() + direction * t);
			nextILast = iTriangle;
			iLast = triangleNormals.count;
		}
	}
	return t;
}

bool SphereTriangleCollision::Detect(
	const Sphere& sphere, const List<Float3>& points,
	Float3& closest)
{
	const float radiusSq{ sphere.GetRadiusSq() };
	for (unsigned iPoint{ 2 }; iPoint < points.GetSize(); iPoint += 3)
	{
		closest = PointTriangleCollision::Closest(
			sphere.GetCenter(),
			&points[iPoint - 2]);

		const Float3 v{ closest - sphere.GetCenter() };
		if (v.LengthSq() <= radiusSq)
			return true;
	}
	return false;
}

bool SphereTriangleCollision::Detect(
	const Sphere& sphere,
	const Float3* pPoints,
	Float3& closest)
{
	closest = PointTriangleCollision::Closest(
		sphere.GetCenter(),
		pPoints);

	const Float3 v{ closest - sphere.GetCenter() };
	if (v.LengthSq() <= sphere.GetRadiusSq())
		return true;
	return false;
}

bool SphereTriangleCollision::Detect(
	const Sphere& sphere, const Float3& direction,
	PtrRangeConst<Float3> points, PtrRangeConst<Float3> triangleNormals,
	MovingSphereCollision& result)
{
	result.T = nextafterf(sphere.GetRadiusSq(), Float::MAX);
	return ContinueDetecting(
		sphere, direction,
		points, triangleNormals,
		result);;
}

/*
	Real-Time Collision Detection book
		5.5.6 Intersecting Moving Sphere Against Triangle (and Polygon)(p.226)
*/
bool SphereTriangleCollision::ContinueDetecting(
	const Sphere& sphere, const Float3& direction,
	PtrRangeConst<Float3> points, PtrRangeConst<Float3> triangleNormals,
	MovingSphereCollision& result)
{
	const float initT{ result.T };

	for (unsigned iPoint{ 2 }, iTriangle{ 0 };
		iPoint < points.count;
		iPoint += 3, iTriangle++)
	{
		const Float3* pTriangle{ &points[iPoint - 2] };
		const Float3& normal{ triangleNormals[iTriangle] };

		if (normal.Dot(direction) > 0)
			continue;

		const Float3 sphereFirstHit{
			sphere.GetCenter() - normal * sphere.GetRadius() }; //point on sphere that will hit plane first

		float t;
		LinePlaneCollision::Detect(
			sphereFirstHit, direction, points[iPoint], normal, t);

		if (t < 0 || t >= result.T)
			continue;

		const Float3 planeFirstHit{
			sphereFirstHit + direction * t }; //point on plane that will be hit first

		Float3 triangleClosest{}; //closest triangle point (to planeFirstHit)
		if (PointTriangleCollision::DetectOrClosest(planeFirstHit, pTriangle, triangleClosest))
		{
			result.HitDirection = -normal;
			result.T = t;
			continue;
		}

		Ray returnRay{ triangleClosest, -direction, result.T };
		if (LineSphereCollision::Detect(returnRay, sphere, t))
		{
			result.HitDirection = triangleClosest - (sphere.GetCenter() + direction * t);
			result.T = t;
		}
	}

	return result.T < initT;
}
