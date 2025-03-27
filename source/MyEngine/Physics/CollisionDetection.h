#pragma once

#include <DataStructures\PtrRangeConst.h>

namespace MyEngine
{
class Sphere;

namespace Physics
{
class CollisionDetection
{
public:
	struct Collision
	{
		Float3 position;
	};

	//--| From, To |-VS-| Points, TriangleNormals |--
	static bool Detect(
		//A
		const Float3& from, const Float3& to,
		//B
		PtrRangeConst<Float3> vertices, PtrRangeConst<Float3> triangleNormals,
		//Result
		Collision& collision);

	template<typename TPoints, typename TNormals>
	static bool Detect(
		//A
		const Float3& from, const Float& to,
		//B
		const TPoints& points, const TNormals& triangleNormals,
		//Result
		Collision& collision);

	//--| From, To |-VS-| Points, TriangleNormals, Indices |--
	static bool Detect(
		//A
		const Float3& from, const Float3& to,
		//B
		PtrRangeConst<Float3> vertices, PtrRangeConst<Float3> triangleNormals, PtrRangeConst<int> indices,
		//Result
		Collision& collision);

	template<typename TPoints, typename TNormals, typename TIndices>
	static bool Detect(
		//A
		const Float3& from, const Float3& to,
		//B
		const TPoints& points, const TNormals& triangleNormals, const TIndices& indices,
		//Result
		Collision& collision);

	//--| Ray |-VS-| Points, TriangleNormals |--
	static bool Detect(
		//A
		const Ray& ray,
		//B
		PtrRangeConst<Float3> vertices, PtrRangeConst<Float3> triangleNormals,
		//Result
		Collision& collision);

	template<typename TPoints, typename TNormals>
	static bool Detect(
		//A
		const Ray& ray,
		//B
		const TPoints& points, const TNormals& triangleNormals,
		//Result
		Collision& collision);

	//--| Sphere |-VS-| Vertices, TriangleNormals, Indices |--
	static bool Detect(
		//A
		const Sphere& sphere,
		//B
		PtrRangeConst<Float3> vertices, PtrRangeConst<Float3> triangleNormals, PtrRangeConst<int> indices,
		//Result
		Float3& overlap);

	template<typename TPoints, typename TNormals, typename TIndices>
	static bool Detect(
		//A
		const Sphere& sphere,
		//B
		const TPoints& points, const TNormals& triangleNormals, const TIndices& indices,
		//Result
		Float3& overlap);

	//---| Other |---
	static bool IsLineInTriangle(
		const Float3& l0, const Float3& l1,
		const Float3& t0, const Float3& t1, const Float3& t2,
		const Float3& tNormal,
		Collision& collision);

private:
	static bool IsPlanePointInTriangle(const Float3& point,
		const Float3& v0, const Float3& v1, const Float3& v2,
		const Float3& triangleNormal);

	static float GetTime(
		const Float3& v0, const Float3& normal,
		const Float3& rayOrigin, const Float3& rayDir);
};
template<typename TPoints, typename TNormals>
inline bool CollisionDetection::Detect(
	const Float3& from, const Float& to,
	const TPoints& points, const TNormals& triangleNormals,
	Collision& collision)
{
	return Detect(
		from, to,
		PtrRangeConst<Float3>{points}, PtrRangeConst<Float3>{triangleNormals},
		collision);
}
template<typename TPoints, typename TNormals, typename TIndices>
inline bool CollisionDetection::Detect(
	const Float3& from, const Float3& to,
	const TPoints& points, const TNormals& triangleNormals, const TIndices& indices,
	Collision& collision)
{
	return Detect(
		from, to,
		PtrRangeConst<Float3>{points}, PtrRangeConst<Float3>{triangleNormals}, PtrRangeConst<int>{indices},
		collision);
}
template<typename TPoints, typename TNormals>
inline bool CollisionDetection::Detect(
	const Ray& ray,
	const TPoints& points, const TNormals& triangleNormals,
	Collision& collision)
{
	return Detect(
		ray,
		PtrRangeConst<Float3>{points}, PtrRangeConst<Float3>{triangleNormals},
		collision);
}
template<typename TPoints, typename TNormals, typename TIndices>
inline bool CollisionDetection::Detect(
	const Sphere& sphere,
	const TPoints& points, const TNormals& triangleNormals, const TIndices& indices,
	Float3& overlap)
{
	return Detect(
		sphere,
		PtrRangeConst<Float3>{points}, PtrRangeConst<Float3>{triangleNormals}, PtrRangeConst<int>{indices},
		overlap);
}
}
}
