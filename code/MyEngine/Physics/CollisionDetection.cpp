#include "pch.h"
#include "CollisionDetection.h"

using namespace Math;

bool Physics::CollisionDetection::Detect(const Float3& from, const Float3& to,
	const Array<Float3>& vertices, const Array<Float3>& normals, const Array<int>& indices, Collision& collision)
{
	float rayLength;
	const Float3 ray{ (to - from).Normalized(rayLength) };

	int iTriangle = 0;
	for (int iIndex = 2; iIndex < indices.GetSize(); iIndex += 3, iTriangle++)
	{
		const Float3& v0{ vertices[indices[iIndex - 2]] };
		const Float3& v1{ vertices[indices[iIndex - 1]] };
		const Float3& v2{ vertices[indices[iIndex]] };
		const Float3& normal{ normals[iTriangle] };

		const Float3 planeToOrigin{ from - v0 };
		const float heightDiff{ planeToOrigin.Dot(normal) };
		const float angle{ -ray.Dot(normal) }; //cos
		const float time{ heightDiff / angle };
		if (time < 0 || time > rayLength) continue;

		const Float3 hitPoint{ from + ray * time };
		if (!IsPlanePointInTriangle(hitPoint, v0, v1, v2, normal)) continue;
		collision.position = hitPoint;
		return true;
	}
	return false;
}

bool Physics::CollisionDetection::IsPlanePointInTriangle(const Float3& point,
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
