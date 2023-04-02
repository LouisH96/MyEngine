#include "pch.h"
#include "ContourOperations.h"

Array<Math::Float2> Io::Ttf::ContourOperations::ToPoints(const Array<TtfPoint>& contourPoints, int nrPointsPerSegment)
{
	if (nrPointsPerSegment < 2)
	{
		Logger::PrintError("Need at least 2 points per segment");
		return{};
	}

	//NR POINTS
	int nrPoints = 0;
	for (int i = 0; i < contourPoints.GetSize(); i++)
	{
		if (contourPoints[i].isOnCurve)
			nrPoints++;
		else
		{
			nrPoints += nrPointsPerSegment - 1;
			if (contourPoints[i + 1].isOnCurve)
				i++;
		}
	}

	//
	Array<Math::Float2> points{ nrPoints };
	int iPoint = 0;
	int iContour = 0;
	while (iContour < contourPoints.GetSize())
	{
		if (contourPoints[iContour].isOnCurve)
		{
			points[iPoint++] = contourPoints[iContour++].position;
			continue;
		}

		const Math::Float2 controlPoint = contourPoints[iContour].position;
		const Math::Float2 prevPoint = points[iPoint - 1];
		Math::Float2 nextPoint;
		if (contourPoints[iContour + 1].isOnCurve)
		{
			nextPoint = contourPoints[iContour + 1].position;
			iContour += 2;
		}
		else
		{
			nextPoint = (contourPoints[iContour].position + contourPoints[iContour + 1].position) / 2;
			iContour += 1;
		}

		const float alphaStep = 1.f / (nrPointsPerSegment - 1);
		for (int iStep = 1; iStep < nrPointsPerSegment; iStep++)
		{
			const Math::Float2 stepPoint = CalculatePoint(prevPoint, controlPoint, nextPoint, iStep * alphaStep);
			points[iPoint++] = stepPoint;
		}
	}
	return points;
}

Math::Float2 Io::Ttf::ContourOperations::CalculatePoint(const Math::Float2& p0, const Math::Float2& p1,
	const Math::Float2& p2, float alpha)
{
	const float invAlpha = 1 - alpha;
	return p0 * (invAlpha * invAlpha)
		+ p1 * (2 * invAlpha * alpha)
		+ p2 * (alpha * alpha);
}
