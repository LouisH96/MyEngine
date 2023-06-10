#include "pch.h"
#include "PlaneGeneration.h"

#include "Rendering/State/Mesh.h"

Array<Rendering::V_PosUv> Generation::PlaneGeneration::TowardsZMin(const Float3& leftBottom, const Float2& size)
{
	const float left{ leftBottom.x };
	const float bot{ leftBottom.y };
	const float right{ left + size.x };
	const float top{ bot + size.y };
	const float z{ leftBottom.z };
	Array<Rendering::V_PosUv> vertices{ 6 };
	vertices[0] = { {left,top,z},{0,0} };
	vertices[1] = { {right,top,z},{1,0} };
	vertices[2] = { {right,bot,z},{1,1} };
	vertices[3] = { {left,top,z},{0,0} };
	vertices[4] = { {right,bot,z},{1,1} };
	vertices[5] = { {left,bot,z},{0,1} };
	return vertices;
}

Rendering::Mesh* Generation::PlaneGeneration::MeshTowardsZMin(const Float3& leftBottom, const Float2& size)
{
	return Rendering::Mesh::Create(TowardsZMin(leftBottom, size));
}
