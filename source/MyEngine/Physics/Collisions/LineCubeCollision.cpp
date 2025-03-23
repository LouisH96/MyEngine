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

bool LineCubeCollision::Detect(const Ray& ray, const Float3& invDirection, const Float3& boxSize)
{
	return Detect(
		ray.Origin, ray.Direction, ray.Length,
		invDirection,
		boxSize);
}

bool LineCubeCollision::Detect(
	const Float3& origin, const Float3& direction, float length,
	const Float3& boxSize)
{
	return Detect(
		origin, direction, length,
		direction.Inversed(),
		boxSize);
}

/*
	RealTime Collision Detection book (p.181)
*/
bool LineCubeCollision::Detect(
	const Float3& origin, const Float3& direction, float length, 
	const Float3& invDirection, 
	const Float3& boxSize)
{
	float tMin{ 0 };
	float tMax{ length };

	for (unsigned iDim{ 0 }; iDim < 3; ++iDim)
	{
		if (abs(direction[iDim]) < 0.00001f)
		{
			//Parallel
			//Origin should be inside slab
			if (origin[iDim] < 0 || origin[iDim] > boxSize[iDim])
				return false;
		}
		else
		{
			float t1{ -origin[iDim] * invDirection[iDim] };
			float t2{ (boxSize[iDim] - origin[iDim]) * invDirection[iDim] };

			if (t1 > t2)
				std::swap(t1, t2);

			Float::UpdateMax(tMin, t1);
			Float::UpdateMin(tMax, t2);

			if (tMin > tMax)
				return false;
		}
	}
	return true;
}
