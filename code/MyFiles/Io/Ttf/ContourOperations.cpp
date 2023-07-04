#include "ContourOperations.h"

#include "Image.h"
#include "TtfReader.h"
#include "DataStructures/Algorithms.h"
#include "DataStructures/DsUtils.h"
#include "Logger/Logger.h"

#undef max;
#undef min;

void MyEngine::Io::Ttf::ContourOperations::GetBounds(const Array<Array<TtfPoint>>& points, Vector2<int16_t>& min,
	Vector2<int16_t>& max)
{
	GetBounds(points[0], min, max, true);
	for (int i = 1; i < points.GetSize(); i++)
		GetBounds(points[i], min, max, false);
}

void MyEngine::Io::Ttf::ContourOperations::GetBounds(const Array<TtfPoint>& points, Vector2<int16_t>& min,
	Vector2<int16_t>& max, bool initMinMax)
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


MyEngine::Array<MyEngine::Double2> MyEngine::Io::Ttf::ContourOperations::ToPoints(const Array<TtfPoint>& contourPoints, int nrPointsPerSegment)
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
	Array<Double2> points{ nrPoints };
	int iPoint = 0;
	int iContour = 0;
	while (iContour < contourPoints.GetSize())
	{
		if (contourPoints[iContour].isOnCurve)
		{
			points[iPoint++] = contourPoints[iContour++].position;
			continue;
		}

		const Double2 controlPoint = contourPoints[iContour].position;
		const Double2 prevPoint = points[iPoint - 1];
		Double2 nextPoint;
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
			const Double2 stepPoint = CalculatePoint(prevPoint, controlPoint, nextPoint, iStep * alphaStep);
			points[iPoint++] = stepPoint;
		}
	}
	return points;
}

MyEngine::Array<MyEngine::Double2> MyEngine::Io::Ttf::ContourOperations::ToPoints(const Segment& segment, int nrPoints)
{
	if (segment.IsLinear())
	{
		Array<Double2> points{ 2 };
		points[0] = { segment.GetBegin() };
		points[1] = { segment.GetEnd() };
		return points;
	}
	if (nrPoints < 2)
	{
		Logger::PrintError("Need at least 2 points for segment");
		return {};
	}
	Array<Double2> points{ nrPoints };
	const float alphaStep{ 1.f / (nrPoints - 1) };
	points[0] = segment.GetBegin();
	for (int i = 1; i < nrPoints; i++)
		points[i] = CalculatePoint(segment, i * alphaStep);
	return points;
}

MyEngine::Array<MyEngine::Double2> MyEngine::Io::Ttf::ContourOperations::ToPoints(const Contour& contour, int nrPointsForCurve)
{
	int nrPoints = 1;
	for (int i = 0; i < contour.GetSegments().GetSize(); i++)
		if (contour.GetSegments()[i].IsLinear())
			nrPoints += 1;
		else
			nrPoints += nrPointsForCurve - 1;

	Array<Double2> points{ nrPoints };
	int iPoint = 0;
	for (int iSegment = 0; iSegment < contour.GetSegments().GetSize(); iSegment++)
	{
		if (contour.GetSegments()[iSegment].IsLinear())
			points[iPoint++] = contour.GetSegments()[iSegment].GetBegin();
		else
		{
			const Array<Double2> segmentPoints{ ToPoints(contour.GetSegments()[iSegment], nrPointsForCurve) };
			for (int iSegmentPoint = 0; iSegmentPoint < segmentPoints.GetSize() - 1; iSegmentPoint++)
				points[iPoint++] = segmentPoints[iSegmentPoint];
		}
	}
	points[points.GetSize() - 1] = points[0];
	return points;
}

MyEngine::Array<MyEngine::Array<MyEngine::Io::Ttf::Segment>> MyEngine::Io::Ttf::ContourOperations::ToSegments(
	const Array<Array<TtfPoint>>& contourPoints)
{
	Array<Array<Segment>> segments{ contourPoints.GetSize() };
	for (int i = 0; i < contourPoints.GetSize(); i++)
		segments[i] = ToSegments(contourPoints[i]);
	return segments;
}

MyEngine::Array<MyEngine::Io::Ttf::Segment> MyEngine::Io::Ttf::ContourOperations::ToSegments(const Array<TtfPoint>& contourPoints)
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
	/*if (contourPoints[contourPoints.GetSize() - 1].isOnCurve)
		nrSegments++;*/

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
					contourPoints[i - 1].position,
					contourPoints[i].position };
			}
			i++;
		}
		else
		{
			const Double2 prevPoint = iSegment == 0 ? Double2{ contourPoints[0].position } : segments[iSegment - 1].GetEnd();
			const Double2 controlPoint = contourPoints[i].position;
			Double2 nextPoint;
			if (contourPoints[i + 1].isOnCurve)
			{
				nextPoint = contourPoints[i + 1].position;
				i += 2;
			}
			else
			{
				nextPoint = (controlPoint + Double2{ contourPoints[i + 1].position }) / 2;
				i++;
			}
			segments[iSegment++] = { prevPoint, controlPoint, nextPoint };
		}
	}
	return segments;
}

MyEngine::Array<MyEngine::Io::Ttf::Intersection> MyEngine::Io::Ttf::ContourOperations::GetIntersectionsX(const Glyph& glyph, float height)
{
	std::vector<Intersection> intersections{};
	for (int iContour = 0; iContour < glyph.GetContours().GetSize(); iContour++)
	{
		const Contour& contour{ glyph.GetContours()[iContour] };
		for (int iSegment = 0; iSegment < contour.GetSegments().GetSize(); iSegment++)
		{
			if (contour.GetSegments()[iSegment].IsLinear())
				AddIntersectionsLinear(contour.GetSegments()[iSegment], height, intersections);
			else
				AddIntersectionsCurve(contour.GetSegments()[iSegment], height, intersections);
		}
	}
	return DsUtils::ToArray(intersections);
}

MyEngine::Array<MyEngine::Io::Ttf::Intersection> MyEngine::Io::Ttf::ContourOperations::GetIntersectionsX(const Contour& contour, float height)
{
	std::vector<Intersection> intersections{};
	for (int i = 0; i < contour.GetSegments().GetSize(); i++)
	{
		if (contour.GetSegments()[i].IsLinear())
			AddIntersectionsLinear(contour.GetSegments()[i], height, intersections);
		else
			AddIntersectionsCurve(contour.GetSegments()[i], height, intersections);
	}
	return DsUtils::ToArray(intersections);
}

MyEngine::Rendering::Image* MyEngine::Io::Ttf::ContourOperations::MakeImage(const TtfReader& reader, char character, int imageWidth,
                                                                            int imageHeight)
{
	Glyph glyph{ reader.GetGlyph(character) };

	const Double2 size{ glyph.GetSize() };
	const double scale{ size.x > size.y ? 1.0 / size.x : 1.0 / size.y };
	glyph.Scale(scale);

	//rasterization
	Rendering::Image* pImage = new Rendering::Image(imageWidth, imageHeight);
	Rasterize(glyph, *pImage);
	return pImage;
}

//temp: contour-segment should have 0-1 scale
void MyEngine::Io::Ttf::ContourOperations::Rasterize(const Glyph& glyph, Rendering::Image& image)
{
	for (int iRow = 0; iRow < image.GetHeight(); iRow++)
	{
		const float y{ 1.f - (static_cast<float>(iRow) - 0.5f) / image.GetHeight() };
		Array<Intersection> intersections = GetIntersectionsX(glyph, y);
		if (intersections.GetSize() < 2) continue;
		Algorithms::SortSmallToBig(intersections);

		/*
		 * debug vis
		 *for (int iIntersection = 0; iIntersection < intersections.GetSize(); iIntersection++)
		{
			const Float3 color{ intersections[iIntersection].isClockwise
				? Float3{0,0,0}
				: Float3{1,1,1} };

			DebugRenderer::AddSphere(Float3{ intersections[iIntersection].distance, y, 0.00f }, color, 0.01f);
		}

		DebugRenderer::AddLine(Float3{ 0,y,-0.01f }, Float3{ 1,y,-0.01f }, Float3{ 1,0,0 });*/

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

MyEngine::Float2 MyEngine::Io::Ttf::ContourOperations::CalculatePoint(const Float2& p0, const Float2& p1,
                                                                      const Float2& p2, float alpha)
{
	const float invAlpha = 1 - alpha;
	return p0 * (invAlpha * invAlpha)
		+ p1 * (2 * invAlpha * alpha)
		+ p2 * (alpha * alpha);
}

MyEngine::Float2 MyEngine::Io::Ttf::ContourOperations::CalculatePoint(const Segment& segment, float alpha)
{
	const float invAlpha = 1 - alpha;
	return segment.GetBegin() * (invAlpha * invAlpha)
		+ segment.GetControlPoint() * (2 * invAlpha * alpha)
		+ segment.GetEnd() * (alpha * alpha);
}

void MyEngine::Io::Ttf::ContourOperations::AddIntersectionsCurve(const Segment& curve, double height,
	std::vector<Intersection>& intersections)
{
	//https://www.wolframalpha.com/input?i=solve+t%2C+y+%3D+a+*+%281-t%29%5E2+%2B+b+*+2+*%281-t%29*t+%2B+c+*+t%5E2
	const double a{ curve.GetBegin().y };
	const double b{ curve.GetControlPoint().y };
	const double c{ curve.GetEnd().y };
	const double divisor{ a - 2 * b + c };
	if (divisor == 0)
		return;
	const double sqrt{ sqrtf(-a * c + a * height + b * b - 2 * b * height + c * height) };
	const double result1{ (sqrt + a - b) / divisor };
	const double result2{ -(sqrt - a + b) / divisor };
	//const bool isClockwise{ (curve.p2 - curve.p0).Cross(curve.p1 - curve.p0) > 0 };
	if (result1 >= 0 && result1 <= 1)
	{
		const Float2 point{ CalculatePoint(curve, result1) };
		intersections.push_back({ point.x, true });
	}
	if (result2 >= 0 && result2 <= 1)
	{
		const Float2 point{ CalculatePoint(curve, result2) };
		intersections.push_back({ point.x, false });
	}
}

void MyEngine::Io::Ttf::ContourOperations::AddIntersectionsLinear(const Segment& linear, double height,
	std::vector<Intersection>& intersections)
{
	const double divisor{ linear.GetEnd().y - linear.GetBegin().y };
	if (divisor == 0)
		return;
	const double result{ (height - linear.GetBegin().y) / divisor };
	if (result >= 0 && result <= 1)
	{
		const Float2 point{ linear.GetBegin() + (linear.GetEnd() - linear.GetBegin()) * result };
		intersections.push_back({ point.x, linear.GetEnd().y > linear.GetBegin().y });
	}
}
