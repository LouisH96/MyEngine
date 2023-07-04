#pragma once
#include <vector>

#include "Intersection.h"
#include "Segment.h"
#include "TtfPoint.h"
#include "DataStructures/Array.h"
#include "Math/Vectors.h"

namespace MyEngine
{
	namespace Rendering
	{
		class Image;
	}
	namespace Io
	{
		class TtfReader;

		namespace Ttf
		{
			class Contour;
			class Glyph;

			class ContourOperations
			{
			public:
				//---| Operations |---
				static void GetBounds(const Array<Array<TtfPoint>>& points, Vector2<int16_t>& min, Vector2<int16_t>& max);
				static void GetBounds(const Array<TtfPoint>& points, Vector2<int16_t>& min, Vector2<int16_t>& max, bool initMinMax = true);

				static Array<Double2> ToPoints(const Array<TtfPoint>& contourPoints, int nrPointsPerSegment = 10);
				static Array<Double2> ToPoints(const Segment& segment, int nrPoints = 10);
				static Array<Double2> ToPoints(const Contour& contour, int nrPointsForCurve = 10);
				static Array<Array<Segment>> ToSegments(const Array<Array<TtfPoint>>& contourPoints);
				static Array<Segment> ToSegments(const Array<TtfPoint>& contourPoints);

				static Array<Intersection> GetIntersectionsX(const Glyph& glyph, float height);
				static Array<Intersection> GetIntersectionsX(const Contour& contour, float height);

				static Rendering::Image* MakeImage(const TtfReader& reader, char character, int imageWidth, int imageHeight);
				static void Rasterize(const Glyph& glyph, Rendering::Image& image);

			private:
				static Float2 CalculatePoint(const Float2& p0, const Float2& p1, const Float2& p2, float alpha);
				static Float2 CalculatePoint(const Segment& segment, float alpha);

				static void AddIntersectionsCurve(const Segment& curve, double height, std::vector<Intersection>& intersections);
				static void AddIntersectionsLinear(const Segment& linear, double height, std::vector<Intersection>& intersections);
			};
		}
	}
}
