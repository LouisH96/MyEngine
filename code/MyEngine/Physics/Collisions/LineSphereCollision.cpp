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
