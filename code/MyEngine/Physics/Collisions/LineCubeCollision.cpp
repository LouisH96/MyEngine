#include "pch.h"
#include "LineCubeCollision.h"

#include "Geometry/Shapes/CubeAA.h"
#include "Geometry/Shapes/Line.h"

using namespace Physics;

bool LineCubeCollision::Detect(const CubeAA& cube, const Line& line)
{
	const float length{ line.GetLength() };
	return Detect(cube, Ray{ line.a, (line.b - line.a) / length, length });
}

bool LineCubeCollision::Detect(const CubeAA& cube, const Ray& ray)
{
	if (!Float::HasOverlap(cube.GetLeft(), cube.GetRight(), ray.Origin.x, ray.GetEnd().x))
		return false;

	float distanceA{ (cube.GetLeft() - ray.Origin.x) / ray.Direction.x };
	float distanceB{ (cube.GetRight() - ray.Origin.x) / ray.Direction.x };
	distanceA = Float::Clamped(distanceA, 0, ray.Length);
	distanceB = Float::Clamped(distanceB, 0, ray.Length);

	const float yA{ ray.Origin.y + ray.Direction.y * distanceA };
	const float yB{ ray.Origin.y + ray.Direction.y * distanceB };
	if (!Float::HasOverlap(cube.GetBot(), cube.GetTop(), yA, yB))
		return false;

	float zA{ ray.Origin.z + ray.Direction.z * distanceA };
	float zB{ ray.Origin.z + ray.Direction.z * distanceB };
	if (!Float::HasOverlap(cube.GetFront(), cube.GetBack(), zA, zB))
		return false;

	distanceA = (cube.GetBot() - ray.Origin.y) / ray.Direction.y;
	distanceB = (cube.GetTop() - ray.Origin.y) / ray.Direction.y;
	distanceA = Float::Clamped(distanceA, 0, ray.Length);
	distanceB = Float::Clamped(distanceB, 0, ray.Length);

	zA = ray.Origin.z + ray.Direction.z * distanceA;
	zB = ray.Origin.z + ray.Direction.z * distanceB;
	return Float::HasOverlap(cube.GetFront(), cube.GetBack(), zA, zB);
}
