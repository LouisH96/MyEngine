#include "pch.h"
#include "LinePlaneCollision.h"

using namespace Physics;

bool LinePlaneCollision::DetectXz(const Ray& ray, float planeY, Float2& hitPoint)
{
	if (ray.Direction.y == 0) return false;
	const float  t{ (planeY - ray.Origin.y) / ray.Direction.y };
	hitPoint = { ray.Origin.x + ray.Direction.x * t, ray.Origin.z + ray.Direction.z * t };
	return true;
}

bool LinePlaneCollision::Detect(const Ray& ray, const Float3& planeOrigin, const Float3& planeRight,
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

bool LinePlaneCollision::Detect(
	const Float3& origin, const Float3& direction, float length,
	const Float3& planePoint, const Float3& planeNormal,
	Float3& hitPoint)
{
	const float distance{
		(origin - planePoint).Dot(planeNormal)
	};

	const float speed{
		direction.Dot(planeNormal)
	};
	if (speed == 0) return false;

	const float time{
		distance / -speed
	};
	if (time < 0 || time > length) return false;

	hitPoint = origin + direction * time;
	return true;
}

void LinePlaneCollision::Detect(
	const Float3& origin, const Float3& direction,
	const Float3& planePoint, const Float3& planeNormal,
	float& t)
{
	const float distance{
		(origin - planePoint).Dot(planeNormal)
	};
	const float speed{
		direction.Dot(planeNormal)
	};
	t = distance / -speed;
}
