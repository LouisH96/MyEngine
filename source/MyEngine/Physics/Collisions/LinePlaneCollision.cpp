#include "pch.h"
#include "LinePlaneCollision.h"

bool Physics::LinePlaneCollision::DetectXz(const Ray& ray, float planeY, Float2& hitPoint)
{
	if (ray.Direction.y == 0) return false;
	const float  t{ (planeY - ray.Origin.y) / ray.Direction.y };
	hitPoint = { ray.Origin.x + ray.Direction.x * t, ray.Origin.z + ray.Direction.z * t };
	return true;
}

bool Physics::LinePlaneCollision::Detect(const Ray& ray, const Float3& planeOrigin, const Float3& planeRight,
	const Float3& planeUp, Float2& hitPoint)
{
	const Float3 normal{ planeUp.Cross(planeRight) };

	const float directionDot{ normal.Dot(ray.Direction) };
	if (directionDot == 0) return false;

	const float originDot{ normal.Dot(ray.Origin - planeOrigin) };
	const float t{ originDot / -directionDot };
	if (t < 0 || t > ray.Length) return false;

	const Float3 hitPoint3{ ray.GetPoint(t) };
	const Float3 toHitPoint3{ hitPoint3 - planeOrigin };

	hitPoint.x = toHitPoint3.Dot(planeRight);
	hitPoint.y = toHitPoint3.Dot(planeUp);
	return true;
}
