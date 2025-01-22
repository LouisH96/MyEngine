#include "pch.h"
#include "CollisionDetection.h"

#include "Geometry/Shapes/Sphere.h"
#include "Geometry/Shapes/Triangle.h"

using namespace Physics;

bool CollisionDetection::Detect(const Float3& from, const Float3& to,
	const Array<Float3>& vertices, const Array<Float3>& triangleNormals, Collision& collision)
{
	float rayLength;
	const Float3 ray{ (to - from).Normalized(rayLength) };

	for (unsigned iVertex = 0, iTriangle = 0; iVertex < vertices.GetSize(); iVertex += 3, iTriangle++)
	{
		const Float3& v0{ vertices[iVertex + 0] };
		const Float3& normal{ triangleNormals[iTriangle] };

		const float time{ GetTime(v0, normal, from, ray) };
		if (time < 0 || time > rayLength) continue;

		const Float3& v1{ vertices[iVertex + 1] };
		const Float3& v2{ vertices[iVertex + 2] };

		const Float3 hitPoint{ from + ray * time };
		if (!IsPlanePointInTriangle(hitPoint, v0, v1, v2, normal)) continue;
		collision.position = hitPoint;
		return true;
	}
	return false;
}

bool CollisionDetection::Detect(const Float3& from, const Float3& to,
	const Array<Float3>& vertices, const Array<Float3>& triangleNormals, const Array<int>& indices, Collision& collision)
{
	float rayLength;
	const Float3 ray{ (to - from).Normalized(rayLength) };

	int iTriangle = 0;
	for (unsigned iIndex = 2; iIndex < indices.GetSize(); iIndex += 3, iTriangle++)
	{
		const Float3& v0{ vertices[indices[iIndex - 2]] };
		const Float3& normal{ triangleNormals[iTriangle] };

		const float time{ GetTime(v0, normal, from, ray) };
		if (time < 0 || time > rayLength) continue;

		const Float3& v1{ vertices[indices[iIndex - 1]] };
		const Float3& v2{ vertices[indices[iIndex]] };

		const Float3 hitPoint{ from + ray * time };
		if (!IsPlanePointInTriangle(hitPoint, v0, v1, v2, normal)) continue;
		collision.position = hitPoint;
		return true;
	}
	return false;
}

bool CollisionDetection::Detect(
	const Sphere& sphere,
	const Array<Float3>& vertices, const Array<Float3>& triangleNormals, const Array<int>& indices,
	Float3& overlap)
{
	for (unsigned iIndex = 2, iTriangle = 0; iIndex < indices.GetSize(); iIndex += 3, iTriangle++)
	{
		const Float3& v2{ vertices[indices[iIndex]] };
		const Float3& normal{ triangleNormals[iTriangle] };

		const Float3 sphereToVertex{ v2 - sphere.GetCenter() };
		const float heightDiff{ sphereToVertex.Dot(normal) };
		if (abs(heightDiff) > sphere.GetRadius()) continue;

		const Float3& v0{ vertices[indices[iIndex - 2]] };
		const Float3& v1{ vertices[indices[iIndex - 1]] };
		if (!IsPlanePointInTriangle(sphere.GetCenter(), v0, v1, v2, normal)) continue;
		overlap = normal * -(sphere.GetRadius() + heightDiff);
		return true;
	}
	return false;
}

bool CollisionDetection::IsLineInTriangle(
	const Float3& l0, const Float3& l1,
	const Float3& t0, const Float3& t1, const Float3& t2,
	const Float3& tNormal,
	Collision& collision)
{
	float rayLength;
	const Float3 ray{ (l1 - l0).Normalized(rayLength) };

	const float time{ GetTime(t0, tNormal, l0, ray) };
	if (time < 0 || time > rayLength) return false;

	const Float3 hitPoint{ l0 + ray * time };
	if (!IsPlanePointInTriangle(hitPoint, t0, t1, t2, tNormal)) return false;
	collision.position = hitPoint;
	return true;
}

bool CollisionDetection::IsPlanePointInTriangle(const Float3& point,
	const Float3& v0, const Float3& v1, const Float3& v2,
	const Float3& triangleNormal)
{
	const Float3 edge01{ v1 - v0 };
	const Float3 v0ToPoint{ point - v0 };
	const Float3 cross01{ edge01.Cross(v0ToPoint) };
	if (cross01.Dot(triangleNormal) < 0) return false;

	const Float3 edge02{ v2 - v0 };
	const Float3 cross02{ edge02.Cross(v0ToPoint) };
	if (cross02.Dot(triangleNormal) > 0) return false;

	const Float3 edge12{ v2 - v1 };
	const Float3 v1ToPoint{ point - v1 };
	const Float3 cross12{ edge12.Cross(v1ToPoint) };
	if (cross12.Dot(triangleNormal) < 0) return false;

	return true;
}

float CollisionDetection::GetTime(
	const Float3& v0, const Float3& normal,
	const Float3& rayOrigin, const Float3& rayDir)
{
	const Float3 planeToOrigin{ rayOrigin - v0 };
	const float heightDiff{ planeToOrigin.Dot(normal) };
	const float angle{ -rayDir.Dot(normal) }; //cos
	return heightDiff / angle;
}
