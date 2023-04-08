#pragma once
#include "Segment.h"
#include "Math/Double2.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			struct TtfPoint;

			class Contour
			{
			public:
				explicit Contour(const Array<TtfPoint>& ttfPoints);

				Contour() = default;
				~Contour() = default;
				Contour(const Contour& other) = default;
				Contour(Contour&& other) noexcept = default;
				Contour& operator=(const Contour& other) = default;
				Contour& operator=(Contour&& other) noexcept = default;

				const Array<Segment>& GetSegments() const { return m_Segments; }
				void Translate(const Math::Double2& translation);
				void Scale(double scale);

				void AddIntersections(std::vector<Intersection>& intersections, double height) const;

				void DebugDraw(const Math::Float3& color, const Math::Float3& offset, int pointsPerCurve) const;
				void DebugPrint();

			private:
				Array<Segment> m_Segments;
			};
		}
	}
}

