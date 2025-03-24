#pragma once
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

	static bool Detect_Rough(const Sphere& sphere, const Float3* pPoint);

	static bool Detect_Rough(const Sphere& sphere, const Array<Float3>& points);

	static bool Detect(const Sphere& sphere, const Array<Float3>& points);

	//Moving
	static bool Detect(
		const Sphere& sphere, const Float3& direction, float amount,
		const Array<Float3>& points, const Array<Float3>& normals);
};
}
}