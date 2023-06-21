#include "pch.h"
#include "PlaneGeneration.h"

#include "Rendering/State/Mesh.h"

Array<Rendering::V_Pos2Uv> Generation::PlaneGeneration::CreateVertices(const Float2& leftBottom, const Float2& size,
	const RectFloat& uvs)
{
	const float left{ leftBottom.x };
	const float bot{ leftBottom.y };
	const float right{ left + size.x };
	const float top{ bot + size.y };
	Array<Rendering::V_Pos2Uv> vertices{ 6 };
	vertices[0] = { {left,top},{uvs.GetLeft(),uvs.GetTopInv()} };
	vertices[1] = { {right,top},{uvs.GetRight(),uvs.GetTopInv()} };
	vertices[2] = { {right,bot},{uvs.GetRight(),uvs.GetBottom()} };
	vertices[3] = { {left,top},{uvs.GetLeft(),uvs.GetTopInv()} };
	vertices[4] = { {right,bot},{uvs.GetRight(),uvs.GetBottom()} };
	vertices[5] = { {left,bot},{uvs.GetLeft(),uvs.GetBottom()} };
	return vertices;
}

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

void Generation::PlaneGeneration::TowardXMin(const Float3& leftBot, const Float2& size, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 right{ 0,0,1 };
	const Float3 up{ 0,1,0 };
	const Float3 normal{ 1,0,0 };
	Create(right, up, normal, leftBot, size, color, vertices, indices);
}

void Generation::PlaneGeneration::TowardXMax(const Float3& leftBot, const Float2& size, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 right{ 0,0,-1 };
	const Float3 up{ 0,1,0 };
	const Float3 normal{ -1,0,0 };
	Create(right, up, normal, leftBot, size, color, vertices, indices);
}

void Generation::PlaneGeneration::TowardYMin(const Float3& leftBot, const Float2& size, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 right{ 1,0,0 };
	const Float3 up{ 0,0,1 };
	const Float3 normal{ 0,1,0 };
	Create(right, up, normal, leftBot, size, color, vertices, indices);
}

void Generation::PlaneGeneration::TowardYMax(const Float3& leftBot, const Float2& size, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 right{ 1,0,0 };
	const Float3 up{ 0,0,-1 };
	const Float3 normal{ 0,-1,0 };
	Create(right, up, normal, leftBot, size, color, vertices, indices);
}

void Generation::PlaneGeneration::TowardZMin(const Float3& leftBot, const Float2& size, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 right{ -1,0,0 };
	const Float3 up{ 0,1,0 };
	const Float3 normal{ 0,0,1 };
	Create(right, up, normal, leftBot, size, color, vertices, indices);
}

void Generation::PlaneGeneration::TowardZMax(const Float3& leftBot, const Float2& size, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 right{ 1,0,0 };
	const Float3 up{ 0,1,0 };
	const Float3 normal{ 0,0,-1 };
	Create(right, up, normal, leftBot, size, color, vertices, indices);
}

void Generation::PlaneGeneration::Create(
	const Float3& right, const Float3& up,
	const Float3& leftBot, const Float2& size, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	Create(right, up, up.Cross(right).Normalized(), leftBot, size, color, vertices, indices);
}

void Generation::PlaneGeneration::Create(
	const Float3& right, const Float3& up, const Float3& normal,
	const Float3& leftBot, const Float2& size, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(3 * 2);

	const int firstIdx{ vertices.GetSize() };

	const Float3 leftTop{ leftBot + up * size.y };
	const Float3 rightTop{ leftTop + right * size.x };
	const Float3 rightBot{ leftBot + right * size.x };

	vertices.Add({ leftBot, color, normal });
	vertices.Add({ leftTop, color, normal });
	vertices.Add({ rightTop, color, normal });
	vertices.Add({ rightBot, color, normal });

	indices.Add(firstIdx, firstIdx + 1, firstIdx + 2);
	indices.Add(firstIdx, firstIdx + 2, firstIdx + 3);
}

void Generation::PlaneGeneration::Create(
	const Float3& leftBot, const Float3& leftTop,
	const Float3& rightTop, const Float3& rightBot, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(3 * 2);
	const int firstIdx{ vertices.GetSize() };
	const Float3 normal{ Float3{leftTop - leftBot}.Cross({rightBot - leftBot}).Normalized() };

	vertices.Add({ leftBot, color, normal });
	vertices.Add({ leftTop, color, normal });
	vertices.Add({ rightTop, color, normal });
	vertices.Add({ rightBot, color, normal });

	indices.Add(firstIdx, firstIdx + 1, firstIdx + 2);
	indices.Add(firstIdx, firstIdx + 2, firstIdx + 3);
}
