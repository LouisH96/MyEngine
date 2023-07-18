#include "pch.h"
#include "SphereGenerator.h"

#include "Shapes.h"

unsigned Generation::SphereGenerator::GetNrVertices(unsigned nrCols, unsigned nrRows)
{
	const int nrMiddlePointRows = nrRows - 1;
	const int nrMiddlePoints = nrMiddlePointRows * nrCols;
	const int nrPoints = nrMiddlePoints + 2;
	return nrPoints;
}

unsigned Generation::SphereGenerator::GetNrIndices(unsigned nrCols, unsigned nrRows)
{
	const unsigned nrPoleTriangles = nrCols * 2;
	const unsigned nrMiddleRowTriangles = nrCols * 2 * (nrRows - 2);
	const unsigned nrTriangles = nrPoleTriangles + nrMiddleRowTriangles;
	return nrTriangles * 3;
}

void Generation::SphereGenerator::Generate(const Sphere& sphere, unsigned nrCols, unsigned nrRows, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	Array<Float3> positions{};
	Array<Float3> normals{};
	Array<int> sphereIndices{};
	Shapes::GenerateSphereBuffers(sphere, nrCols, nrRows, positions, normals, sphereIndices);

	indices.EnsureIncrease(sphereIndices.GetSizeU());
	for (unsigned i = 0, begin = vertices.GetSizeU(); i < sphereIndices.GetSizeU(); i++)
		indices.Add(sphereIndices[i] + begin);

	vertices.EnsureIncrease(positions.GetSizeU());
	for (unsigned i = 0; i < positions.GetSizeU(); i++)
		vertices.Add({ positions[i], color, normals[i] });
}
