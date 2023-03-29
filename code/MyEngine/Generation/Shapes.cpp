#include "pch.h"
#include "Shapes.h"
#include "DataStructures/Array.h"
#include "Math/Constants.h"
#include "Math/Cube.h"
#include <Generation/Shapes/ArrowGenerator.h>

void Generation::Shapes::GenerateCubeBuffers(const Math::Cube& cube, Ds::Array<Math::Float3>& positions,
	Ds::Array<Math::Float3>& normals, Ds::Array<int>& indices)
{
	using namespace Math;
	constexpr int trianglesPerSide = 2;
	constexpr int pointsPerSide = 4;
	constexpr int indicesPerSide = trianglesPerSide * 3;
	constexpr int nrSides = 6;
	constexpr int nrVertices = pointsPerSide * nrSides;
	positions = Ds::Array<Float3>{ nrVertices };
	normals = Ds::Array<Float3>{ nrVertices };
	indices = Ds::Array<int>{ indicesPerSide * nrSides };

	for (int iAxis = 0; iAxis < Float3::NR_AXIS; iAxis++)
	{
		Float3 right, up, depth;
		right.Set(1.f, (iAxis + 1) % Float3::NR_AXIS);
		up.Set(1.f, (iAxis + 2) % Float3::NR_AXIS);
		depth.Set(1.f, iAxis);
		right.Scale(cube.GetSize());
		up.Scale(cube.GetSize());
		depth.Scale(cube.GetSize());

		const int idxFront = iAxis * pointsPerSide * 2;
		const int idxBack = idxFront + pointsPerSide;

		//front
		positions[idxFront + 0] = cube.GetOrigin();
		positions[idxFront + 1] = positions[idxFront + 0] + up;
		positions[idxFront + 2] = positions[idxFront + 1] + right;
		positions[idxFront + 3] = positions[idxFront + 0] + right;

		//back
		positions[idxBack + 0] = positions[idxFront + 0] + depth;
		positions[idxBack + 1] = positions[idxFront + 1] + depth;
		positions[idxBack + 2] = positions[idxFront + 2] + depth;
		positions[idxBack + 3] = positions[idxFront + 3] + depth;

		//normals
		Float3 normal;
		normal.Set(-1.f, (iAxis + 3) % Float3::NR_AXIS);
		for (int iVertex = 0; iVertex < pointsPerSide; iVertex++)
		{
			normals[idxFront + iVertex] = normal;
			normals[idxBack + iVertex] = -normal;
		}

		//indices front
		const int idxIndicesFront = iAxis * indicesPerSide * 2;
		const int idxIndicesBack = idxIndicesFront + indicesPerSide;
		indices[idxIndicesFront + 0] = idxFront + 0;
		indices[idxIndicesFront + 1] = idxFront + 1;
		indices[idxIndicesFront + 2] = idxFront + 2;

		indices[idxIndicesFront + 3] = idxFront + 0;
		indices[idxIndicesFront + 4] = idxFront + 2;
		indices[idxIndicesFront + 5] = idxFront + 3;

		//indices back
		indices[idxIndicesBack + 0] = idxBack + 0;
		indices[idxIndicesBack + 1] = idxBack + 2;
		indices[idxIndicesBack + 2] = idxBack + 1;

		indices[idxIndicesBack + 3] = idxBack + 0;
		indices[idxIndicesBack + 4] = idxBack + 3;
		indices[idxIndicesBack + 5] = idxBack + 2;
	}

}

void Generation::Shapes::GenerateSphereBuffers(const Sphere& sphere, int nrCols, int nrRows, Array<Float3>& positions,
	Array<Float3>& normals, Array<int>& indices)
{
	const float anglePerCol = Constants::PI / (static_cast<float>(nrCols) / 2);
	const float anglePerRow = Constants::PI / static_cast<float>(nrRows);
	const int nrMiddlePointRows = nrRows - 1;
	const int nrMiddlePoints = nrMiddlePointRows * nrCols;
	const int nrPoints = nrMiddlePoints + 2;

	//---| Points |---
	positions = Array<Float3>{ nrPoints };
	normals = Array<Float3>(nrPoints);

	//top & bot
	const Float3 bot = sphere.GetCenter() - Float3{ 0.f, sphere.GetRadius(), 0.f };
	const Float3 top = sphere.GetCenter() + Float3{ 0.f, sphere.GetRadius(), 0.f };
	positions[0] = bot;
	normals[0] = { 0,-1,0 };
	positions[positions.GetSize() - 1] = top;
	normals[normals.GetSize() - 1] = { 0,1,0 };

	//middle points
	for (int iRow = 0; iRow < nrMiddlePointRows; iRow++)
	{
		const float rowAngle = static_cast<float>(iRow + 1) * anglePerRow;
		const float rowRadius = sinf(rowAngle) * sphere.GetRadius();
		for (int iCol = 0; iCol < nrCols; iCol++)
		{
			const float colAngle = static_cast<float>(iCol) * anglePerCol;
			const Float3 localPos = {
				cosf(colAngle) * rowRadius,
				-cosf(rowAngle) * sphere.GetRadius(),
				sinf(colAngle) * rowRadius
			};
			positions[iRow * nrCols + iCol + 1] = sphere.GetCenter() + localPos;
			normals[iRow * nrCols + iCol + 1] = localPos.Normalized();
		}
	}

	//---| Indices |--
	const int nrPoleTriangles = nrCols * 2;
	const int nrMiddleRowTriangles = nrCols * 2 * (nrRows - 2);
	const int nrTriangles = nrPoleTriangles + nrMiddleRowTriangles;
	const int nrIndices = nrTriangles * 3;
	indices = { nrIndices };

	//bot pole
	for (int i = 0; i < nrCols; i++)
	{
		const int idxLeft = i + 1;
		int idxRight = i + 2;
		if (idxRight > nrCols)
			idxRight = 1;

		indices[i * 3] = 0;
		indices[i * 3 + 1] = idxLeft;
		indices[i * 3 + 2] = idxRight;
	}
	//top pole
	for (int i = 0; i < nrCols; i++)
	{
		const int leftIdx = positions.GetSize() - 2 - i;
		int rightIdx = positions.GetSize() - 3 - i;
		if (i == nrCols - 1)
			rightIdx = positions.GetSize() - 2;

		indices[nrIndices - 1 - (i * 3)] = positions.GetSize() - 1;
		indices[nrIndices - 2 - (i * 3)] = rightIdx;
		indices[nrIndices - 3 - (i * 3)] = leftIdx;
	}
	//middle triangles
	for (int iRow = 1; iRow < nrRows - 1; iRow++)
	{
		for (int iCol = 0; iCol < nrCols; iCol++)
		{
			const int leftBottom = (iRow - 1) * nrCols + iCol + 1;
			const int rightBottom = iCol < nrCols - 1 ? leftBottom + 1 : leftBottom - (nrCols - 1);
			const int leftTop = leftBottom + nrCols;
			const int rightTop = rightBottom + nrCols;

			int* pIndex = &indices[(nrCols + ((iRow - 1) * nrCols + iCol) * 2) * 3];
			*pIndex++ = leftBottom;
			*pIndex++ = leftTop;
			*pIndex++ = rightTop;

			*pIndex++ = leftBottom;
			*pIndex++ = rightTop;
			*pIndex = rightBottom;
		}
	}
}

void Generation::Shapes::GeneratePivotArrows(Array<Rendering::V_PosColNorm>& vertices, Array<int>& indices)
{
	using namespace Rendering;
	constexpr float totalLength = 1.f;
	constexpr float arrowLength = .2f;
	constexpr float arrowRadius = .1f;
	constexpr int nrArrowsTriangle = 6;
	
	Array<Float3> positions{};
	Array<Float3> normals{};
	const ArrowGenerator arrowGenerator{ nrArrowsTriangle, false, .8f, .05f, .2f, .1f };
	arrowGenerator.Generate(positions, normals, indices);
	vertices = { positions.GetSize() };
	for (int i = 0; i < vertices.GetSize(); i++)
		vertices[i] = V_PosColNorm{ positions[i], {1,0,0},normals[i] };
}

Array<int> Generation::Shapes::GetIndicesToCapCircle(int nrPoints)
{
	if (nrPoints < 4)
	{
		Logger::PrintError("CapCircle function need at least 4 points");
		return{};
	}
	if (nrPoints % 2 != 0)
	{
		Logger::PrintError("CapCircle function need even nr of points");
		return{};
	}

	Array<int> indices{ (nrPoints - 2) * 3 };
	constexpr int topIdx = 0;
	const int botIdx = nrPoints / 2;
	const int rightIdx = nrPoints / 4;
	const int leftIdx = rightIdx + botIdx;

	//LEFT CORNER
	indices[indices.GetSize() - 3] = leftIdx - 1;
	indices[indices.GetSize() - 2] = leftIdx;
	indices[indices.GetSize() - 1] = leftIdx + 1;
	//RIGHT CORNER
	indices[indices.GetSize() - 6] = rightIdx - 1;
	indices[indices.GetSize() - 5] = rightIdx;
	indices[indices.GetSize() - 4] = rightIdx + 1;
	//MIDDLE QUADS
	for (int i = 0; i < rightIdx - 1; i++)
	{
		//RIGHT
		const int rLeftTop = i;
		const int rRightTop = i + 1;
		const int rLeftBot = botIdx - i;
		const int rRightBot = botIdx - i - 1;
		const int idx = i * 4 * 3;
		indices[idx + 0] = rLeftTop;
		indices[idx + 1] = rRightTop;
		indices[idx + 2] = rRightBot;
		indices[idx + 3] = rLeftTop;
		indices[idx + 4] = rRightBot;
		indices[idx + 5] = rLeftBot;

		//LEFT
		const int lLeftTop = nrPoints - i - 1;
		const int lRightTop = i == 0 ? 0 : nrPoints - i;
		const int lLeftBot = botIdx + i + 1;
		const int lRightBot = botIdx + i;
		indices[idx + 6] = lLeftTop;
		indices[idx + 7] = lRightTop;
		indices[idx + 8] = lRightBot;
		indices[idx + 9] = lLeftTop;
		indices[idx + 10] = lRightBot;
		indices[idx + 11] = lLeftBot;
	}
	return indices;
}
