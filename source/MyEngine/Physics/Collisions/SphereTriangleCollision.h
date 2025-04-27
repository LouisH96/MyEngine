#pragma once

#include <DataStructures\PtrRangeConst.h>
#include <Geometry\Shapes\Sphere.h>
#include <Geometry\Shapes\Triangle.h>

namespace MyEngine::Physics
{
/*
	Detect_Rough simply checks if one of the corners is inside the sphere.
*/
class SphereTriangleCollision
{
public:

	//--| Sphere |-VS-| Point* |--
	static bool Detect(
		//A
		const Sphere& sphere,
		//B
		const Float3* pPoints,
		//Result
		Float3& closest);

	static bool Detect_Rough(
		//A
		const Sphere& sphere,
		//B
		const Float3* pPoint);

	//--| Sphere |-VS-| Point Collection |--
	static bool Detect(
		//A
		const Sphere& sphere,
		//B
		const List<Float3>& points,
		//Result
		Float3& closest);

	static bool Detect_Rough(
		//A
		const Sphere& sphere,
		//B
		const List<Float3>& points);

	static float MoveOut(
		//A
		const Sphere& sphere,
		const Float3& direction,
		//B
		PtrRangeConst<Float3> points, PtrRangeConst<Float3> triangleNormals);

	//---| Moving Sphere |---

	//--| Sphere, Direction, Length |-VS-| Points, TriangleNormals |--
	struct MovingSphereCollision
	{
		Float3 HitDirection;
		float T;
	};

	/*
		Does not detect triangles starting inside the sphere! (by design)
		Result::HitDirection might not be normalized!
	*/
	static bool Detect(
		//A
		const Sphere& sphere, const Float3& direction,
		//B
		PtrRangeConst<Float3> points, PtrRangeConst<Float3> triangleNormals,
		//Result
		MovingSphereCollision& result
	);
	/*
		Difference with Detect is this one takes
			an active/initialized Result struct.
	*/
	static bool ContinueDetecting(
		//A
		const Sphere& sphere, const Float3& direction,
		//B
		PtrRangeConst<Float3> points, PtrRangeConst<Float3> triangleNormals,
		//Result
		MovingSphereCollision& result
	);

	template<typename TPoints, typename TNormals>
	static bool Detect(
		//A
		const Sphere& sphere, const Float3& direction, float length,
		//B
		const TPoints& points, const TNormals& triangleNormals,
		//Result
		MovingSphereCollision& result);
};

template<typename TPoints, typename TNormals>
inline bool SphereTriangleCollision::Detect(
	const Sphere& sphere, const Float3& direction, float length,
	const TPoints& points, const TNormals& normals,
	//Result
	MovingSphereCollision& result)
{
	return Detect(
		sphere, direction, length,
		PtrRangeConst<Float3>{points}, PtrRangeConst<Float3>{normals},
		result);
}

}