#include "pch.h"
#include "LineOnLineCollision.h"

#include "Geometry/Shapes/Line.h"

bool Physics::LineOnLineCollision::Detect(const Line& lineA, const Line& lineB, Float3& hitPoint)
{
	const Float3 aDisplacement{ lineA.b - lineA.a };
	const float aLength{ aDisplacement.Length() };
	const Float3 aDirection{ aDisplacement / aLength };

	const Float3 bDisplacement{ lineB.b - lineB.a };
	const float bLength{ bDisplacement.Length() };
	const Float3 bDirection{ bDisplacement / bLength };

	const float angle{ aDirection.Dot(bDirection) };
	if (angle == 0) return false;

	const Float3 toB{ lineB.a - lineA.a };
	const float dot{ aDirection.Dot(toB) };
	const Float3 dotPoint{ lineA.a + aDirection * dot };

	const Float3 bToDot{ dotPoint - lineB.a };

	const float tB{ bToDot.LengthSq() / bDirection.Dot(bToDot) };
	if (tB < 0 || tB > bLength) return false;

	const float tA{ angle * tB + dot };
	if (tA < 0 || tA > aLength) return false;

	hitPoint = lineA.a + aDirection * tA;
	return true;
}

