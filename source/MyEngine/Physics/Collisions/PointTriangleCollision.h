#pragma once
#include <Math\Vectors.h>

namespace MyEngine
{
class PointTriangleCollision
{
public:
	static Float3 Closest(const Float3& point, const Float3* pTrianglePoints);
};
}