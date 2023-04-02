#include "pch.h"
#include "ContourOperations.h"

bool Io::Ttf::ContourOperations::Segment::IsLinear() const
{
	return p1.x == INFINITY || p1.y == INFINITY;
}

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

Array<Math::Float2> Io::Ttf::ContourOperations::ToPoints(const Segment& segment, int nrPoints)
{
	if (segment.IsLinear())
	{
		Array<Math::Float2> points{ 2 };
		points[0] = { segment.p0 };
		points[1] = { segment.p2 };
		return points;
	}
	if (nrPoints < 2)
	{
		Logger::PrintError("Need at least 2 points for segment");
		return {};
	}
	Array<Math::Float2> points{ nrPoints };
	const float alphaStep{ 1.f / (nrPoints - 1) };
	points[0] = segment.p0;
	for (int i = 1; i < nrPoints; i++)
		points[i] = CalculatePoint(segment, i * alphaStep);
	return points;
}

Array<Math::Float2> Io::Ttf::ContourOperations::ToPoints(const Array<Segment>& segments, int nrPointsForCurve)
{
	int nrPoints = 1;
	for (int i = 0; i < segments.GetSize(); i++)
		if (segments[i].IsLinear())
			nrPoints += 1;
		else
			nrPoints += nrPointsForCurve - 1;

	Array<Math::Float2> points{ nrPoints };
	int iPoint = 0;
	for (int iSegment = 0; iSegment < segments.GetSize(); iSegment++)
	{
		if (segments[iSegment].IsLinear())
			points[iPoint++] = segments[iSegment].p0;
		else
		{
			const Array<Math::Float2> segmentPoints{ ToPoints(segments[iSegment], nrPointsForCurve) };
			for (int iSegmentPoint = 0; iSegmentPoint < segmentPoints.GetSize() - 1; iSegmentPoint++)
				points[iPoint++] = segmentPoints[iSegmentPoint];
		}
	}
	points[points.GetSize() - 1] = points[0];
	return points;
}

Array<Io::Ttf::ContourOperations::Segment> Io::Ttf::ContourOperations::ToSegments(const Array<TtfPoint>& contourPoints)
{
	int nrSegments = 0;
	for (int i = 1; i < contourPoints.GetSize(); i++)
	{
		if (contourPoints[i].isOnCurve)
		{
			if (contourPoints[i - 1].isOnCurve)
				nrSegments++;
		}
		else
			nrSegments++;
	}
	if (contourPoints[contourPoints.GetSize() - 1].isOnCurve)
		nrSegments++;

	//
	Array<Segment> segments{ nrSegments };
	int iSegment = 0;
	for (int i = 1; i < contourPoints.GetSize();)
	{
		if (contourPoints[i].isOnCurve)
		{
			if (contourPoints[i - 1].isOnCurve)
			{
				segments[iSegment++] = Segment{
					contourPoints[i - 1].position, {INFINITY, INFINITY},
					contourPoints[i].position };
			}
			i++;
		}
		else
		{
			const Math::Float2 prevPoint = iSegment == 0 ? Math::Float2{ contourPoints[0].position } : segments[iSegment - 1].p2;
			const Math::Float2 controlPoint = contourPoints[i].position;
			Math::Float2 nextPoint;
			if (contourPoints[i + 1].isOnCurve)
			{
				nextPoint = contourPoints[i + 1].position;
				i += 2;
			}
			else
			{
				nextPoint = (controlPoint + Math::Float2{ contourPoints[i + 1].position }) / 2;
				i++;
			}
			segments[iSegment++] = { prevPoint, controlPoint, nextPoint };
		}
	}
	return segments;
}

Math::Float2 Io::Ttf::ContourOperations::CalculatePoint(const Math::Float2& p0, const Math::Float2& p1,
	const Math::Float2& p2, float alpha)
{
	const float invAlpha = 1 - alpha;
	return p0 * (invAlpha * invAlpha)
		+ p1 * (2 * invAlpha * alpha)
		+ p2 * (alpha * alpha);
}

Math::Float2 Io::Ttf::ContourOperations::CalculatePoint(const Segment& segment, float alpha)
{
	const float invAlpha = 1 - alpha;
	return segment.p0 * (invAlpha * invAlpha)
		+ segment.p1 * (2 * invAlpha * alpha)
		+ segment.p2 * (alpha * alpha);
}
