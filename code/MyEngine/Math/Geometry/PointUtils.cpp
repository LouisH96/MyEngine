#include "pch.h"
#include "PointUtils.h"

void Math::Geometry::PointUtils::Translate(Ds::Array<Float3>& points, const Float3& translation)
{
	for (int i = 0; i < points.GetSize(); i++)
		points[i] += translation;
}
