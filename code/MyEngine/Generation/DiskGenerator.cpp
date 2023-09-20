#include "pch.h"
#include "DiskGenerator.h"

using namespace Generation;

unsigned DiskGenerator::GetNrVertices(const Options& options)
{
	return options.NrPoints;
}

unsigned DiskGenerator::GetNrIndices(const Options& options)
{
	return (options.NrPoints - 2) * 3;
}

Array<Float3> DiskGenerator::GenerateXz(const Float3& center, float radius, unsigned nrPoints)
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
