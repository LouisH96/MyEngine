#pragma once
#include <Math\Vectors.h>

namespace MyEngine
{
class PointTriangleCollision
{
public:
	static Float3 Closest(const Float3& point, const Float3* pTrianglePoints);

	static bool Detect(const Float3& point, const Float3* pTriangle);

	static bool Detect2(const Float3& point, const Float3* pTriangle);

	/*
		Only fills closest when not inside triangle
	*/
	static bool DetectOrClosest(const Float3& point, const Float3* pTriangle, Float3& closest);
};
}