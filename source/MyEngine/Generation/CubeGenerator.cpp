#include "pch.h"
#include "CubeGenerator.h"

#include "PlaneGeneration.h"
#include "Geometry/Shapes/Cube.h"
#include "Geometry/Shapes/CubeAA.h"

using namespace Generation;
using namespace Rendering;

void CubeGenerator::FrontPlane(const CubeAA& cube, const Float3& color,
                               List<V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetLeftBotFront() + offset };
	const Float2 size{ cube.GetXySize() };
	PlaneGeneration::TowardZMin(leftBot, size, color, vertices, indices);
}

void CubeGenerator::BackPlane(const CubeAA& cube, const Float3& color,
                              List<V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetRightBotBack() + offset };
	const Float2 size{ cube.GetXySize() };
	PlaneGeneration::TowardZMax(leftBot, size, color, vertices, indices);
}

void CubeGenerator::LeftPlane(const CubeAA& cube, const Float3& color,
                              List<V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetLeftBotBack() + offset };
	const Float2 size{ cube.GetZySize() };
	PlaneGeneration::TowardXMin(leftBot, size, color, vertices, indices);
}

void CubeGenerator::RightPlane(const CubeAA& cube, const Float3& color,
                               List<V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetRightBotFront() + offset };
	const Float2 size{ cube.GetZySize() };
	PlaneGeneration::TowardXMax(leftBot, size, color, vertices, indices);
}

void CubeGenerator::TopPlane(const CubeAA& cube, const Float3& color, List<V_PosColNorm>& vertices,
                             List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetLeftTopFront() + offset };
	const Float2 size{ cube.GetXzSize() };
	PlaneGeneration::TowardYMax(leftBot, size, color, vertices, indices);
}

void CubeGenerator::BottomPlane(const CubeAA& cube, const Float3& color,
                                List<V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetLeftBotBack() + offset };
	const Float2 size{ cube.GetXzSize() };
	PlaneGeneration::TowardYMin(leftBot, size, color, vertices, indices);
}

void CubeGenerator::Generate(const CubeAA& cube, const Float3& color, List<V_PosColNorm>& vertices,
                             List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4 * CubeAA::NR_SIDES);
	indices.EnsureIncrease(3 * 2 * CubeAA::NR_SIDES);

	FrontPlane(cube, color, vertices, indices, offset);
	BackPlane(cube, color, vertices, indices, offset);
	LeftPlane(cube, color, vertices, indices, offset);
	RightPlane(cube, color, vertices, indices, offset);
	TopPlane(cube, color, vertices, indices, offset);
	BottomPlane(cube, color, vertices, indices, offset);
}

void CubeGenerator::FrontPlane(const Cube& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
	List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(3 * 2);
	PlaneGeneration::Create(
		cube.GetRight(), cube.GetUp(),
		cube.GetPosition() + offset, { cube.GetSize().x, cube.GetSize().y },
		color, vertices, indices);
}

void CubeGenerator::BackPlane(const Cube& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
	List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(3 * 2);
	PlaneGeneration::Create(
		-cube.GetRight(), cube.GetUp(),
		cube.GetRightBotBack() + offset, { cube.GetSize().x, cube.GetSize().y },
		color, vertices, indices);
}

void CubeGenerator::LeftPlane(const Cube& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
	List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(3 * 2);
	PlaneGeneration::Create(
		-cube.GetForward(), cube.GetUp(),
		cube.GetLeftBotBack() + offset, { cube.GetSize().z, cube.GetSize().y },
		color, vertices, indices);
}

void CubeGenerator::RightPlane(const Cube& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
	List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(3 * 2);
	PlaneGeneration::Create(
		cube.GetForward(), cube.GetUp(),
		cube.GetRightBotFront() + offset, { cube.GetSize().z, cube.GetSize().y },
		color, vertices, indices);
}

void CubeGenerator::TopPlane(const Cube& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
	List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(3 * 2);
	PlaneGeneration::Create(
		cube.GetRight(), cube.GetForward(),
		cube.GetLeftTopFront() + offset, { cube.GetSize().x, cube.GetSize().z },
		color, vertices, indices);
}

void CubeGenerator::BottomPlane(const Cube& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
	List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(3 * 2);
	PlaneGeneration::Create(
		cube.GetRight(), -cube.GetForward(),
		cube.GetLeftBotBack() + offset, { cube.GetSize().x, cube.GetSize().z },
		color, vertices, indices);
}

void CubeGenerator::Generate(const Cube& cube, const Float3& color,
                             List<V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4 * Cube::NR_SIDES);
	indices.EnsureIncrease(3 * 2 * Cube::NR_SIDES);

	FrontPlane(cube, color, vertices, indices, offset);
	BackPlane(cube, color, vertices, indices, offset);
	LeftPlane(cube, color, vertices, indices, offset);
	RightPlane(cube, color, vertices, indices, offset);
	TopPlane(cube, color, vertices, indices, offset);
	BottomPlane(cube, color, vertices, indices, offset);
}
