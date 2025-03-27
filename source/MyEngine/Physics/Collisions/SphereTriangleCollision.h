#pragma once

#include <DataStructures\PtrRangeConst.h>
#include <Geometry\Shapes\Sphere.h>
#include <Geometry\Shapes\Triangle.h>

namespace MyEngine
{
namespace Physics
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
		const Float3* pPoints);

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
		const List<Float3>& points);

	static bool Detect_Rough(
		//A
		const Sphere& sphere,
		//B
		const List<Float3>& points);

	//---| Moving Sphere |---

	//--| Sphere, Direction, Length |-VS-| Points, TriangleNormals |--
	static bool Detect(
		//A
		const Sphere& sphere, const Float3& direction, float length,
		//B
		PtrRangeConst<Float3> points, PtrRangeConst<Float3> triangleNormals);

	template<typename TPoints, typename TNormals>
	static bool Detect(
		//A
		const Sphere& sphere, const Float3& direction, float length,
		//B
		const TPoints& points, const TNormals& triangleNormals);
};

template<typename TPoints, typename TNormals>
inline bool SphereTriangleCollision::Detect(
	const Sphere& sphere, const Float3& direction, float length,
	const TPoints& points, const TNormals& normals)
{
	return Detect(
		sphere, direction, length,
		PtrRangeConst<Float3>{points}, PtrRangeConst<Float3>{normals});
}

}
}