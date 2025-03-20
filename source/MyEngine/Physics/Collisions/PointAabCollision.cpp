#include "pch.h"
#include "PointAabCollision.h"

Float3 PointAabCollision::Closest(Float3 point, const Float3& boxSize)
{
	for (unsigned iDim{ 0 }; iDim < 3; ++iDim)
	{
		if (point[iDim] < 0)
			point[iDim] = 0;
		else if (point[iDim] > boxSize[iDim])
			point[iDim] = boxSize[iDim];
	}
	return point;
}
