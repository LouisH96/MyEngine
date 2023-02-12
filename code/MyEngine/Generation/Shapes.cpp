#include "pch.h"
#include "Shapes.h"
#include "DataStructures/Array.h"
#include "Math/Cube.h"

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
		for(int iVertex = 0; iVertex < pointsPerSide; iVertex++)
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
