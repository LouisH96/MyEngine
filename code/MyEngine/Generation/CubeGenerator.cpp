#include "pch.h"
#include "CubeGenerator.h"

#include "PlaneGeneration.h"

void Generation::CubeGenerator::FrontPlane(const Cube& cube, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 leftBot{ cube.GetRightBotFront() };
	const Float2 size{ cube.GetXySize() };
    PlaneGeneration::TowardZMin(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::BackPlane(const Cube& cube, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 leftBot{ cube.GetLeftBotBack() };
	const Float2 size{ cube.GetXySize() };
	PlaneGeneration::TowardZMax(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::LeftPlane(const Cube& cube, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 leftBot{ cube.GetLeftBotFront() };
	const Float2 size{ cube.GetZySize() };
	PlaneGeneration::TowardXMax(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::RightPlane(const Cube& cube, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 leftBot{ cube.GetRightBotBack() };
	const Float2 size{ cube.GetZySize() };
	PlaneGeneration::TowardXMin(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::TopPlane(const Cube& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
	List<int>& indices)
{
	const Float3 leftBot{ cube.GetLeftTopBack() };
	const Float2 size{ cube.GetXzSize() };
	PlaneGeneration::TowardYMin(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::BottomPlane(const Cube& cube, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices)
{
	const Float3 leftBot{ cube.GetLeftBotFront() };
	const Float2 size{ cube.GetXzSize() };
	PlaneGeneration::TowardYMax(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::Generate(const Cube& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
                                         List<int>& indices)
{
	vertices.EnsureIncrease(4 * Cube::NR_SIDES);
	indices.EnsureIncrease(3 * 2 * Cube::NR_SIDES);

	FrontPlane(cube, color, vertices, indices);
	BackPlane(cube, color, vertices, indices);
	LeftPlane(cube, color, vertices, indices);
	RightPlane(cube, color, vertices, indices);
	TopPlane(cube, color, vertices, indices);
	BottomPlane(cube, color, vertices, indices);
}
