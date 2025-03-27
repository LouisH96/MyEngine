#include "pch.h"
#include "LineSphereCollision.h"

using namespace Physics;

bool LineSphereCollision::Detect(const Ray& ray, const Sphere& sphere)
{
	//todo: uncomplete
	const Float3 toCenter{ sphere.GetCenter() - ray.Origin };
	const float dot{ ray.Direction.Dot(toCenter) };
	if (dot < 0 || dot > ray.Length) return false;
	const Float3 closest{ ray.GetPoint(dot) };

	return closest.DistanceSq(sphere.GetCenter()) <= sphere.GetRadiusSq();
}

/*
	Real-Time Collision Detection book.
		p.178
*/
bool LineSphereCollision::Detect(const Ray& ray, const Sphere& sphere, float& t)
{
	const Float3 m{ ray.Origin - sphere.GetCenter() };
	const float b{ m.Dot(ray.Direction) };
	const float c{ m.LengthSq() - sphere.GetRadiusSq() };

	if (c > 0 && b > 0)
		return false;

	const float discr{ b * b - c };
	if (discr < 0)
		return false;

	t = -b - sqrtf(discr);
	if (t > ray.Length)
		return false;
	
	if (t < 0)
		t = 0;

	return true;
}

int LineSphereCollision::Detect(const Ray& ray, PtrRangeConst<Sphere> spheres)
{
	int idx{ -1 };
	float closest{ Float::Max() };
	for (unsigned i = 0; i < spheres.count; i++)
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
