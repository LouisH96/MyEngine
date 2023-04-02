#pragma once
#include "TtfPoint.h"
#include "Math/Float2.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class ContourOperations
			{
			public:
				//---| Types |---
				struct Segment
				{
					Math::Float2 p0;
					Math::Float2 p1; //infinity if linear segment
					Math::Float2 p2;

					bool IsLinear() const;
				};

				//---| Operations |---
				static Array<Math::Float2> ToPoints(const Array<TtfPoint>& contourPoints, int nrPointsPerSegment = 10);
				static Array<Math::Float2> ToPoints(const Segment& segment, int nrPoints = 10);
				static Array<Math::Float2> ToPoints(const Array<Segment>& segments, int nrPointsForCurve = 10);
				static Array<Segment> ToSegments(const Array<TtfPoint>& contourPoints);

			private:
				static Math::Float2 CalculatePoint(const Math::Float2& p0, const Math::Float2& p1, const Math::Float2& p2, float alpha);
				static Math::Float2 CalculatePoint(const Segment& segment, float alpha);
			};
		}
	}
}
