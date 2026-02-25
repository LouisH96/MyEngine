#include "pch.h"
#include "Bounds.h"

namespace MyEngine::Geometry
{
void Bounds::GetCylinderBoundsXZ(const Io::Fbx::FbxClass& fbx, float& radius, float& height)
{
	using namespace Io::Fbx;

	radius = 0;
	height = 0;

	for (unsigned iGeom{ 0 }; iGeom < fbx.GetGeometries().GetSize(); ++iGeom)
	{
		const FbxClass::Geometry& geom{ fbx.GetGeometry(iGeom) };
		for (unsigned iPoint{ 0 }; iPoint < geom.Points.GetSize(); ++iPoint)
		{
			const Float3& point{ geom.Points[iPoint] };
			const float currRadius{ point.x * point.x + point.z * point.z };
			const float currHeight{ point.y };
			if (currRadius > radius) radius = currRadius;
			if (currHeight > height) height = currHeight;
		}
	}
	if (radius > 0)
		radius = sqrtf(radius);
}
}
