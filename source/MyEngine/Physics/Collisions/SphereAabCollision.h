#pragma once
#include <Geometry\Shapes\Sphere.h>

namespace MyEngine
{
class SphereAabCollision
{
public:
	static bool Detect(const Sphere& sphere, const Float3& boxSize);
};
}