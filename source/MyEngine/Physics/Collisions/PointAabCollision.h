#pragma once
#include <Math\Vectors.h>

namespace MyEngine
{
class PointAabCollision
{
public:
	static Float3 Closest(Float3 point, const Float3& boxSize);
};
}