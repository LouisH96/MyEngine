#include "pch.h"
#include "MeshUtils.h"

//#define MESH_UTILS_DEBUG

void Generation::MeshUtils::CapCircle(int nrPoints, int arrayOffset, int valueOffset, Array<int>& indices)
{
	if (nrPoints % 2 != 0)
	{
		Logger::PrintError("NrPoints should be even");
		return;
	}
	if (arrayOffset + GetNrTrianglesToCapCircle(nrPoints) * 3 >= indices.GetSize())
	{
		Logger::PrintError("Indices-Array is not big enough for adding cap-circle indices");
		return;
	}
	const int half = nrPoints / 2;

	int iIndex = arrayOffset;
	int iPoint = 0;
	while (iPoint < nrPoints)
	{
		int opposite = half - iPoint;
		if (opposite < 0)
			opposite += nrPoints;

		const int next = iPoint == nrPoints - 1 ? 0 : iPoint + 1;

		//corner?
		if (opposite == next )
		{
			opposite++;
			if (opposite == nrPoints)
				opposite = 0;
			indices[iIndex++] = iPoint + valueOffset;
			indices[iIndex++] = next + valueOffset;
			indices[iIndex++] = opposite + valueOffset;
			iPoint = opposite;
			if (iPoint == 0)
				break;
		}
		else
		{
			indices[iIndex++] = iPoint++ + valueOffset;
			indices[iIndex++] = next + valueOffset;
			indices[iIndex++] = opposite + valueOffset;
			if (next + 1 == opposite)
				iPoint++;
		}
	}
}

int Generation::MeshUtils::GetNrTrianglesToCapCircle(int nrCirclePoints)
{
#ifdef MESH_UTILS_DEBUG
	AssertCapCirclePoints(nrCirclePoints);
#endif
	return nrCirclePoints - 2;
}

void Generation::MeshUtils::AssertCapCirclePoints(int nrCirclePoints)
{
	if (nrCirclePoints < 4)
		Logger::PrintError("Need at least 4 points to cap a circle");
	if (nrCirclePoints % 2 != 0)
		Logger::PrintError("Number of points need to be even to cap a circle");
}
