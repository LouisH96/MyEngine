#pragma once
#include "Intersection.h"
#include "Segment.h"
#include "TtfPoint.h"
#include "Math/Float2.h"

namespace MyEngine
{
	namespace Io
	{
		class TtfReader;
	}
}

namespace MyEngine
{
	namespace Rendering
	{
		class Image;
	}
}

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class Contour;
			class Glyph;

			class ContourOperations
			{
			public:
				//---| Operations |---
				static void GetBounds(const Array<Array<TtfPoint>>& points, Math::Value2<int16_t>& min, Math::Value2<int16_t>& max);
				static void GetBounds(const Array<TtfPoint>& points, Math::Value2<int16_t>& min, Math::Value2<int16_t>& max, bool initMinMax = true);

				static Array<Math::Double2> ToPoints(const Array<TtfPoint>& contourPoints, int nrPointsPerSegment = 10);
				static Array<Math::Double2> ToPoints(const Segment& segment, int nrPoints = 10);
				static Array<Math::Double2> ToPoints(const Contour& contour, int nrPointsForCurve = 10);
				static Array<Array<Segment>> ToSegments(const Array<Array<TtfPoint>>& contourPoints);
				static Array<Segment> ToSegments(const Array<TtfPoint>& contourPoints);

				static Array<Intersection> GetIntersectionsX(const Glyph& glyph, float height);
				static Array<Intersection> GetIntersectionsX(const Contour& contour, float height);

				static Rendering::Image* MakeImage(const Io::TtfReader& reader, char character, int imageWidth, int imageHeight);
				static void Rasterize(const Glyph& glyph, Rendering::Image& image);

			private:
				static Math::Float2 CalculatePoint(const Math::Float2& p0, const Math::Float2& p1, const Math::Float2& p2, float alpha);
				static Math::Float2 CalculatePoint(const Segment& segment, float alpha);

				static void AddIntersectionsCurve(const Segment& curve, double height, std::vector<Intersection>& intersections);
				static void AddIntersectionsLinear(const Segment& linear, double height, std::vector<Intersection>& intersections);
			};
		}
	}
}
