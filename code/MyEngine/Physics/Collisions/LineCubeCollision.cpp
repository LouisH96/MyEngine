#include "pch.h"
#include "LineCubeCollision.h"

#include "Geometry/Shapes/Cube.h"
#include "Geometry/Shapes/CubeAA.h"
#include "Geometry/Shapes/Line.h"

using namespace Physics;

bool LineCubeCollision::Detect(const Line& line, const CubeAA& cube)
{
	const float length{ line.GetLength() };
	return Detect(Ray{ line.a, (line.b - line.a) / length, length }, cube);
}

bool LineCubeCollision::Detect(const Line& line, const Cube& cube)
{
	const float length{ line.GetLength() };
	const Float3 direction{ (line.b - line.a) / length };
	Ray relativeRay{ line.a - cube.GetPosition(), direction, length };
	const Quaternion inverseRotation{ -cube.GetTransform().Rotation };
	inverseRotation.RotatePoint(relativeRay.Origin);
	inverseRotation.RotatePoint(relativeRay.Direction);
	return Detect(relativeRay, CubeAA{ {}, cube.GetSize() });
}

bool LineCubeCollision::Detect(const Ray& ray, const CubeAA& cube)
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
