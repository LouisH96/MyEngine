#include "pch.h"
#include "SphereAabCollision.h"

#include "PointAabCollision.h"

bool SphereAabCollision::Detect(
	const Sphere& sphere, const Float3& boxSize)
{
	const Float3 closest{ PointAabCollision::Closest(
		sphere.GetCenter(), boxSize) };

	return closest.DistanceSq(sphere.GetCenter())
		<= sphere.GetRadiusSq();
}
