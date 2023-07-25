#include "pch.h"
#include "Cone.h"

Cone::Cone(const Float3& origin, const Float3& direction, float length, float radius)
	: origin{ origin }
	, direction{ direction }
	, length{ length }
	, radius{ radius }
{
}

Float3 Cone::GetEnd() const
{
	return origin + direction * length;
}

Float3 Cone::GetRight() const
{
	if (direction == Float3{0, 1, 0})
		return { 1,0,0 };
	return Float3{ 0,1,0 }.Cross(direction).NormalizedSafe();
}

Float3 Cone::GetUp(const Float3& right) const
{
	return direction.Cross(right).Normalized();
}

float Cone::GetAngle() const
{
	return atan2f(radius, length);
}
