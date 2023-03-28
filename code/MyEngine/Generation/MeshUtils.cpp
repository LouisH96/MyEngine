#include "pch.h"
#include "MeshUtils.h"

//#define MESH_UTILS_DEBUG

using namespace Math;

void Generation::MeshUtils::GetIndicesToCapCircle(int nrPoints, Array<int>& indices, int arrayOffset, int valueOffset)
{
	if (nrPoints < 4)
	{
		Logger::PrintError("CapCircle function need at least 4 points");
		return;
	}
	if (nrPoints % 2 != 0)
	{
		Logger::PrintError("CapCircle function need even nr of points");
		return;
	}
	if (arrayOffset + GetNrTrianglesToCapCircle(nrPoints) * 3 >= indices.GetSize())
	{
		Logger::PrintError("Indices-Array is not big enough for adding cap-circle indices");
		return;
	}
	const int nrIndices = GetNrTrianglesToCapCircle(nrPoints)*3;
	const int topIdx = 0 + valueOffset;
	const int botIdx = nrPoints / 2 + valueOffset;
	const int rightIdx = nrPoints / 4 + valueOffset;
	const int leftIdx = nrPoints / 4 + botIdx;

	//LEFT CORNER
	indices[arrayOffset + nrIndices - 3] = leftIdx - 1;
	indices[arrayOffset + nrIndices - 2] = leftIdx;
	indices[arrayOffset + nrIndices - 1] = leftIdx + 1;
	//RIGHT CORNER
	indices[arrayOffset + nrIndices - 6] = rightIdx - 1;
	indices[arrayOffset + nrIndices - 5] = rightIdx;
	indices[arrayOffset + nrIndices - 4] = rightIdx + 1;
	//MIDDLE QUADS
	for (int i = 0; i < rightIdx - 1 - valueOffset; i++)
	{
		//RIGHT
		const int rLeftTop = topIdx + i;
		const int rRightTop = topIdx + i + 1;
		const int rLeftBot = botIdx - i;
		const int rRightBot = botIdx - i - 1;
		const int idx = arrayOffset + i * 4 * 3;
		indices[idx + 0] = rLeftTop;
		indices[idx + 1] = rRightTop;
		indices[idx + 2] = rRightBot;
		indices[idx + 3] = rLeftTop;
		indices[idx + 4] = rRightBot;
		indices[idx + 5] = rLeftBot;

		//LEFT
		const int lLeftTop = topIdx + nrPoints - i - 1;
		const int lRightTop = i == 0 ? topIdx : topIdx + nrPoints - i;
		const int lLeftBot = botIdx + i + 1;
		const int lRightBot = botIdx + i;
		indices[idx + 6] = lLeftTop;
		indices[idx + 7] = lRightTop;
		indices[idx + 8] = lRightBot;
		indices[idx + 9] = lLeftTop;
		indices[idx + 10] = lRightBot;
		indices[idx + 11] = lLeftBot;
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
