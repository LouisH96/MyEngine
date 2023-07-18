#include "pch.h"
#include "LineSphereCollision.h"

#include "Geometry/Shapes/Ray.h"
#include "Geometry/Shapes/Sphere.h"

bool Physics::LineSphereCollision::Detect(const Ray& ray, const Sphere& sphere)
{
	const Float3 toCenter{ sphere.GetCenter() - ray.Origin };
	const float dot{ ray.Direction.Dot(toCenter) };
	if (dot < 0 || dot > ray.Length) return false;
	const Float3 closest{ ray.GetPoint(dot) };

	return closest.DistanceSq(sphere.GetCenter()) <= sphere.GetRadiusSq();
}

int Physics::LineSphereCollision::Detect(const Ray& ray, PtrRangeConst<Sphere> spheres)
{
	int idx{ -1 };
	float closest{ Float::Max() };
	for (int i = 0; i < spheres.count; i++)
	{
		const Sphere& sphere{ spheres.pData[i] };
		const Float3 toCenter{ sphere.GetCenter() - ray.Origin };
		const float dot{ ray.Direction.Dot(toCenter) };
		if (dot < 0 || dot > ray.Length) continue;
		const Float3 closestPoint{ ray.GetPoint(dot) };
		const float distanceSq{ closestPoint.DistanceSq(sphere.GetCenter()) };
		if (distanceSq > sphere.GetRadiusSq()) continue;
		const float distance{ sqrtf(distanceSq) };
		const float angleSin{ distance / sphere.GetRadius() };
		const float angleCos{ cosf(asinf(angleSin)) };
		const float distanceInside{ angleCos * sphere.GetRadius() };
		const float t{ dot - distanceInside };
		if (t >= closest) continue;
		idx = i;
		closest = t;
	}
	return idx;
}
