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
				static Array<Math::Float2> ToPoints(const Array<TtfPoint>& contourPoints, int nrPointsPerSegment = 10);

			private:
				static Math::Float2 CalculatePoint(const Math::Float2& p0, const Math::Float2& p1, const Math::Float2& p2, float alpha);
			};
		}
	}
}
