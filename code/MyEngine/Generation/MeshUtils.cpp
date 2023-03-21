#include "pch.h"
#include "MeshUtils.h"
#include <Math/Float3.h>
#include <DataStructures/DsUtils.h>

using namespace Math;

void Generation::MeshUtils::MakePointAndNormalBuffersFromFbxIndices(const Array<int>& indices,
	const Array<Math::Float3>& points, Array<Math::Float3>& outputPoints, Array<Math::Float3>& outputNormals)
{
	std::vector<Float3> outputVector{};

	for (int iIndex = 0; iIndex < indices.GetSize();)
	{
		const int index0 = indices[iIndex++];
		int index1 = indices[iIndex++];
		int index2 = indices[iIndex++];

		outputVector.push_back(points[index0]);
		outputVector.push_back(points[index1]);

		while(index2 >= 0)
		{
			outputVector.push_back(points[index2]);
			index1 = index2;
			index2 = indices[iIndex++];
			outputVector.push_back(points[index0]);
			outputVector.push_back(points[index1]);
		}
		outputVector.push_back(points[-index2 - 1]);
	}
	outputPoints = Ds::DsUtils::ToArray(outputVector);

	//normals
	outputNormals = { outputPoints.GetSize() };
	for (int iPoint = 0; iPoint < outputPoints.GetSize(); iPoint += 3)
	{
		const Float3& point0{ outputPoints[iPoint + 0] };
		const Float3& point1{ outputPoints[iPoint + 1] };
		const Float3& point2{ outputPoints[iPoint + 2] };

		const Float3 edge1{ point1 - point0 };
		const Float3 edge2{ point2 - point0 };
		const Float3 normal{ edge1.Cross(edge2).Normalized() };
		outputNormals[iPoint + 0] = normal;
		outputNormals[iPoint + 1] = normal;
		outputNormals[iPoint + 2] = normal;

	}
}
