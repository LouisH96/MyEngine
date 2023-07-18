#include "pch.h"
#include "CubeGenerator.h"

#include "PlaneGeneration.h"
#include "Geometry/Shapes/CubeAA.h"

void Generation::CubeGenerator::FrontPlane(const CubeAA& cube, const Float3& color,
                                           List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetRightBotFront() + offset };
	const Float2 size{ cube.GetXySize() };
	PlaneGeneration::TowardZMin(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::BackPlane(const CubeAA& cube, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetLeftBotBack() + offset };
	const Float2 size{ cube.GetXySize() };
	PlaneGeneration::TowardZMax(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::LeftPlane(const CubeAA& cube, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetLeftBotFront() + offset };
	const Float2 size{ cube.GetZySize() };
	PlaneGeneration::TowardXMax(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::RightPlane(const CubeAA& cube, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetRightBotBack() + offset };
	const Float2 size{ cube.GetZySize() };
	PlaneGeneration::TowardXMin(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::TopPlane(const CubeAA& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
	List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetLeftTopBack() + offset };
	const Float2 size{ cube.GetXzSize() };
	PlaneGeneration::TowardYMin(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::BottomPlane(const CubeAA& cube, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const Float3 leftBot{ cube.GetLeftBotFront() + offset };
	const Float2 size{ cube.GetXzSize() };
	PlaneGeneration::TowardYMax(leftBot, size, color, vertices, indices);
}

void Generation::CubeGenerator::Generate(const CubeAA& cube, const Float3& color, List<Rendering::V_PosColNorm>& vertices,
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
