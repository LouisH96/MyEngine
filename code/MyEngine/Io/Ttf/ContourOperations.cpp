#include "pch.h"
#include "ContourOperations.h"

#include "Image.h"
#include "TtfReader.h"
#include "DataStructures/Algorithms.h"
#include "DataStructures/DsUtils.h"
#include <Debug/DebugRenderer.h>

#undef max;
#undef min;

bool Io::Ttf::ContourOperations::Segment::IsLinear() const
{
	return p1.x == INFINITY || p1.y == INFINITY;
}

bool Io::Ttf::ContourOperations::Intersection::operator<(const Intersection& other) const
{
	return distance < other.distance;
}

bool Io::Ttf::ContourOperations::Intersection::operator>(const Intersection& other) const
{
	return distance > other.distance;
}

void Io::Ttf::ContourOperations::GetBounds(const Array<Array<TtfPoint>>& points, Math::Value2<int16_t>& min,
	Math::Value2<int16_t>& max)
{
	GetBounds(points[0], min, max, true);
	for (int i = 1; i < points.GetSize(); i++)
		GetBounds(points[i], min, max, false);
}

void Io::Ttf::ContourOperations::GetBounds(const Array<TtfPoint>& points, Math::Value2<int16_t>& min,
	Math::Value2<int16_t>& max, bool initMinMax)
{
	if (initMinMax)
	{
		min.x = std::numeric_limits<int16_t>::max();
		min.y = min.x;
		max.x = std::numeric_limits<int16_t>::min();
		max.y = max.x;
	}
	for (int i = 0; i < points.GetSize(); i++)
	{
		if (points[i].position.x < min.x)min.x = points[i].position.x;
		if (points[i].position.x > max.x)max.x = points[i].position.x;
		if (points[i].position.y < min.y)min.y = points[i].position.y;
		if (points[i].position.y > max.y)max.y = points[i].position.y;
	}
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

Array<Array<Io::Ttf::ContourOperations::Segment>> Io::Ttf::ContourOperations::ToSegments(
	const Array<Array<TtfPoint>>& contourPoints)
{
	Array<Array<Segment>> segments{ contourPoints.GetSize() };
	for (int i = 0; i < contourPoints.GetSize(); i++)
		segments[i] = ToSegments(contourPoints[i]);
	return segments;
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

Array<Io::Ttf::ContourOperations::Intersection> Io::Ttf::ContourOperations::GetIntersectionsX(const Array<Array<Segment>>& contours, float height)
{
	std::vector<Intersection> intersections{};
	for (int iContour = 0; iContour < contours.GetSize(); iContour++)
	{
		const Array<Segment>& segments{ contours[iContour] };
		for (int iSegment = 0; iSegment < segments.GetSize(); iSegment++)
		{
			if (segments[iSegment].IsLinear())
				AddIntersectionsLinear(segments[iSegment], height, intersections);
			else
				AddIntersectionsCurve(segments[iSegment], height, intersections);
		}
	}
	return DsUtils::ToArray(intersections);
}

Array<Io::Ttf::ContourOperations::Intersection> Io::Ttf::ContourOperations::GetIntersectionsX(const Array<Segment>& segments, float height)
{
	std::vector<Intersection> intersections{};
	for (int i = 0; i < segments.GetSize(); i++)
	{
		if (segments[i].IsLinear())
			AddIntersectionsLinear(segments[i], height, intersections);
		else
			AddIntersectionsCurve(segments[i], height, intersections);
	}
	return DsUtils::ToArray(intersections);
}

Rendering::Image* Io::Ttf::ContourOperations::MakeImage(const TtfReader& reader, char character, int imageWidth,
	int imageHeight)
{
	const Array<Array<TtfPoint>> contourPoints{ reader.GetPoints(character) };
	Array<Array<Segment>> contourSegments{ ToSegments(contourPoints) };

	//scale
	for (int iContour = 0; iContour < contourSegments.GetSize(); iContour++)
	{
		Array<Segment>& contour{ contourSegments[iContour] };
		for (int iSegment = 0; iSegment < contour.GetSize(); iSegment++)
		{
			Segment& segment = contour[iSegment];
			segment.p0 /= 1422.f;
			segment.p2 /= 1422.f;
			if (!segment.IsLinear())
				segment.p1 /= 1422.f;
		}
	}

	//rasterization
	Rendering::Image* pImage = new Rendering::Image(imageWidth, imageHeight);
	Rasterize(contourSegments, *pImage);
	return pImage;
}

//temp: contour-segment should have 0-1 scale
void Io::Ttf::ContourOperations::Rasterize(const Array<Array<Segment>>& contourSegments, Rendering::Image& image)
{
	for (int iRow = 0; iRow < image.GetHeight(); iRow++)
	{
		const float y{ 1.f - (static_cast<float>(iRow) - 0.5f) / image.GetHeight() };
		Array<Intersection> intersections = GetIntersectionsX(contourSegments, y);
		if (intersections.GetSize() < 2) continue;
		Algorithms::SortSmallToBig(intersections);

		/*
		 * debug vis
		 *for (int iIntersection = 0; iIntersection < intersections.GetSize(); iIntersection++)
		{
			const Math::Float3 color{ intersections[iIntersection].isClockwise
				? Math::Float3{0,0,0}
				: Math::Float3{1,1,1} };

			DebugRenderer::AddSphere(Math::Float3{ intersections[iIntersection].distance, y, 0.00f }, color, 0.01f);
		}

		DebugRenderer::AddLine(Math::Float3{ 0,y,-0.01f }, Math::Float3{ 1,y,-0.01f }, Math::Float3{ 1,0,0 });*/

		int windingCounter = 0; //zero-winding
		int lastCol = 0;
		for (int iIntersection = 0; iIntersection < intersections.GetSize(); iIntersection++)
		{
			const int newCol = static_cast<int>(roundf(intersections[iIntersection].distance * image.GetWidth()));
			if (windingCounter != 0)
			{
				//draw
				for (int iCol = lastCol; iCol < newCol; iCol++)
					image.SetColor(iCol, iRow, 255, 0, 0, 255);
			}
			lastCol = newCol;
			windingCounter += intersections[iIntersection].rightIsInside ? 1 : -1;
		}
	}
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

void Io::Ttf::ContourOperations::AddIntersectionsCurve(const Segment& curve, float height,
	std::vector<Intersection>& intersections)
{
	//https://www.wolframalpha.com/input?i=solve+t%2C+y+%3D+a+*+%281-t%29%5E2+%2B+b+*+2+*%281-t%29*t+%2B+c+*+t%5E2
	const float a{ curve.p0.y };
	const float b{ curve.p1.y };
	const float c{ curve.p2.y };
	const float divisor{ a - 2 * b + c };
	if (divisor == 0)
		return;
	const float sqrt{ sqrtf(-a * c + a * height + b * b - 2 * b * height + c * height) };
	const float result1{ (sqrt + a - b) / divisor };
	const float result2{ -(sqrt - a + b) / divisor };
	//const bool isClockwise{ (curve.p2 - curve.p0).Cross(curve.p1 - curve.p0) > 0 };
	if (result1 >= 0 && result1 <= 1)
	{
		const Math::Float2 point{ CalculatePoint(curve, result1) };
		intersections.push_back({ point.x, true });
	}
	if (result2 >= 0 && result2 <= 1)
	{
		const Math::Float2 point{ CalculatePoint(curve, result2) };
		intersections.push_back({ point.x, false });
	}
}

void Io::Ttf::ContourOperations::AddIntersectionsLinear(const Segment& linear, float height,
	std::vector<Intersection>& intersections)
{
	const float divisor{ linear.p2.y - linear.p0.y };
	if (divisor == 0)
		return;
	const float result{ (height - linear.p0.y) / divisor };
	if (result >= 0 && result <= 1)
	{
		const Math::Float2 point{ linear.p0 + (linear.p2 - linear.p0) * result };
		intersections.push_back({ point.x, linear.p2.y > linear.p0.y });
	}
}
