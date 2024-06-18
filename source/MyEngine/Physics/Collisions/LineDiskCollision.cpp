#include "pch.h"
#include "LineDiskCollision.h"

using namespace Physics;

bool LineDiskCollision::Detect(const Ray& ray, const Float3& diskCenter, const Float3& diskNormal,
	float diskRadiusSq, float& t)
{
	const Float3 toDisk{ diskCenter - ray.Origin };
	const float height{ toDisk.Dot(diskNormal) };
	const float heightStep{ ray.Direction.Dot(diskNormal) };
	t = height / heightStep;
	const Float3 hitPoint{ ray.GetPoint(t) };
	return hitPoint.DistanceSq(diskCenter) <= diskRadiusSq;
}
