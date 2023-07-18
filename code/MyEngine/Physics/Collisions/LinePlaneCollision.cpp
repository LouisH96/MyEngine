#include "pch.h"
#include "LinePlaneCollision.h"

bool Physics::LinePlaneCollision::DetectXz(const Ray& ray, float planeY, Float2& hitPoint)
{
	if (ray.Direction.y == 0) return false;
	const float  t{ (planeY - ray.Origin.y) / ray.Direction.y };
	hitPoint = { ray.Origin.x + ray.Direction.x * t, ray.Origin.z + ray.Direction.z * t };
	return true;
}
