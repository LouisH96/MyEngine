#pragma once
#include "Math/Double2.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class Segment
			{
			public:
				Segment() = default;
				Segment(const Math::Double2& begin, const Math::Double2& controlPoint, const Math::Double2& end);
				Segment(const Math::Double2& begin, const Math::Double2& end);
				~Segment() = default;
				Segment(const Segment& other) = default;
				Segment(Segment&& other) noexcept = default;
				Segment& operator=(const Segment& other) = default;
				Segment& operator=(Segment&& other) noexcept = default;

				bool IsLinear() const;
				const Math::Double2& GetBegin() const { return m_Begin; }
				const Math::Double2& GetEnd() const { return m_End; }
				const Math::Double2& GetControlPoint() const { return m_ControlPoint; }

				Math::Double2 CalculatePoint(double alpha) const;
				void AddCurvePoints(Array<Math::Double2>& points, int offset, int pointsPerCurve) const;
				void AddLinearPoints(Array<Math::Double2>& points, int offset) const;

				void Translate(const Math::Double2& translation);
				void Scale(double scale);

				void DebugPrint() const;

			private:
				Math::Double2 m_Begin;
				Math::Double2 m_End;
				Math::Double2 m_ControlPoint;
				bool m_IsLinear;
			};
		}
	}
}
