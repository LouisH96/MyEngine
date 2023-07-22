#include "pch.h"
#include "DiskGenerator.h"

Array<Float3> Generation::DiskGenerator::GenerateXz(const Float3& center, float radius, unsigned nrPoints)
{
	Array<Float3> points{nrPoints};
	const float angleStep{ Constants::PI2 / static_cast<float>(nrPoints) };
	for (unsigned i = 0; i < nrPoints; i++)
	{
		const float angle{ angleStep * i };
		points[i] = center + Float3{cosf(angle)* radius, 0, sinf(angle)* radius};
	}
	return points;
}
