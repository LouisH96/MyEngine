#pragma once
#include <vector>

#include "DataStructures/Array.h"
#include "Math/Vectors.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			struct Intersection;

			class Segment
			{
			public:
				Segment() = default;
				Segment(const Double2& begin, const Double2& controlPoint, const Double2& end);
				Segment(const Double2& begin, const Double2& end);
				~Segment() = default;
				Segment(const Segment& other) = default;
				Segment(Segment&& other) noexcept = default;
				Segment& operator=(const Segment& other) = default;
				Segment& operator=(Segment&& other) noexcept = default;

				bool IsLinear() const;
				const Double2& GetBegin() const { return m_Begin; }
				const Double2& GetEnd() const { return m_End; }
				const Double2& GetControlPoint() const { return m_ControlPoint; }

				Double2 CalculatePoint(double alpha) const;
				void AddCurvePoints(Array<Double2>& points, int offset, int pointsPerCurve) const;
				void AddLinearPoints(Array<Double2>& points, int offset) const;

				void Translate(const Double2& translation);
				void Scale(double scale);

				void AddIntersectionPoints(std::vector<Intersection>& intersections, double height) const;

				void DebugPrint() const;

			private:
				Double2 m_Begin;
				Double2 m_End;
				Double2 m_ControlPoint;
				bool m_IsLinear;

				void AddIntersectionPointsLinear(std::vector<Intersection>& intersections, double height) const;
				void AddIntersectionPointsCurve(std::vector<Intersection>& intersections, double height) const;
			};
		}
	}
}
