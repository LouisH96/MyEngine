#include "pch.h"
#include "Ray.h"

Float3 Ray::GetPoint(float distance) const
{
	return Origin + Direction * distance;
}
