#pragma once
#include <Geometry/Shapes/TriangleBase.h>

namespace MyEngine
{
	template<typename Point>
	class TriangleCollisionDetection
	{
	public:
		static bool Detect(const TriangleBase<Point>& a, const TriangleBase<Point>& b);

	private:
		static bool InternalDetect(const TriangleBase<Point>& a, const TriangleBase<Point>& b);
	};

	template<typename Point>
	inline bool TriangleCollisionDetection<Point>::Detect(const TriangleBase<Point>& a, const TriangleBase<Point>& b)
	{
		return InternalDetect(a, b) && InternalDetect(b, a);
	}

	template<typename Point>
	inline bool TriangleCollisionDetection<Point>::InternalDetect(const TriangleBase<Point>& a, const TriangleBase<Point>& b)
	{
		using Triangle = TriangleBase<Point>;
		for (unsigned iSide = 0; iSide < Triangle::NR_POINTS; iSide++)
		{
			const Point& refLineBegin{ a.GetPoint(iSide) };
			const Point& refLineEnd{ a.GetPoint((iSide + 1) % Triangle::NR_POINTS) };

			const Point refPerp{ refLineEnd.y - refLineBegin.y, refLineBegin.x - refLineEnd.x };

			typename Point::Unit sum{ 0 };
			for (unsigned i = 0; i < Triangle::NR_POINTS; i++)
			{
				const Point toBPoint{ b.GetPoint(i) - refLineBegin };
				sum += Scalar<Point::Unit>::Sign(refPerp.Dot(toBPoint));
			}

			if (-sum == Triangle::NR_POINTS)
				return false;
		}

		return true;
	}

}