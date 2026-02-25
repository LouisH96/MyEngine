#pragma once

#include <Io\Fbx\FbxClass.h>

namespace MyEngine::Geometry
{
class Bounds
{
public:
	static void GetCylinderBoundsXZ(const Io::Fbx::FbxClass& fbx, float& radius, float& height);
};
}