#include "pch.h"
#include "Ray.h"

Ray::Ray(const Float3& origin, const Float3& direction, float length)
	: Origin{ origin }
	, Direction{ direction }
	, Length{ length }
{
}

Ray::Ray(const Float3& from, const Float3& to)
	: Origin{ from }
{
	Direction = (to - from).Normalized(Length);
}

Float3 Ray::GetPoint(float distance) const
{
	return Origin + Direction * distance;
}

Float3 Ray::GetEnd() const
{
	return Origin + Direction * Length;
}
