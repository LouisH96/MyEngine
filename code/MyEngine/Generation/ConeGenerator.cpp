#include "pch.h"
#include "ConeGenerator.h"

#include "Geometry/Shapes/Cone.h"

using namespace Generation;
using namespace Rendering;

void ConeGenerator::Generate(
	const Cone& cone, const Float3& color, unsigned nrPoints,
	List<V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(nrPoints + 1);
	indices.EnsureIncrease(nrPoints * 3);
	const unsigned first{ vertices.GetSizeU() };

	const float coneAngle{ cone.GetAngle() };
	const float coneCos{ cosf(coneAngle) };
	const float coneSin{ sinf(coneAngle) };

	const Float3 origin{ cone.origin + offset };
	const Float3 end{ cone.GetEnd() + offset };
	const Float3 right{ cone.GetRight() };
	const Float3 up{ cone.GetUp(right) };
	const float angleStep{ Constants::PI2 / Float::Cast(nrPoints) };

	vertices.Add({ origin, color, {} });

	for (float i = 0; i < Float::Cast(nrPoints); i++)
	{
		const float angle1{ i * angleStep };

		Float3 pos1{ right * cosf(angle1) + up * sinf(angle1) };
		const Float3 normal1{ pos1 * coneCos - cone.direction * coneSin };
		pos1 *= cone.radius;
		pos1 += end;

		vertices.Add(V_PosColNorm{ pos1, color, normal1 });
	}

	for (unsigned i = 0; i < nrPoints; i++)
	{
		const unsigned idx1{ i + first + 1 };
		const unsigned idx2{ (i + 1 == nrPoints ? 0 : i + 1) + first + 1 };
		indices.Add(first, idx2, idx1);
	}
}
