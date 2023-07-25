#include "pch.h"
#include "ConeGenerator.h"

#include "MeshUtils.h"
#include "Geometry/Shapes/Cone.h"

using namespace Generation;
using namespace Rendering;

void ConeGenerator::Generate(
	const Cone& cone, const Float3& color, unsigned nrPoints,
	List<V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	const int nrCapTriangles{ MeshUtils::GetNrTrianglesToCapCircle(nrPoints) };

	const unsigned first{ vertices.GetSizeU() };
	vertices.IncreaseSize(1 + nrPoints * 2);
	indices.EnsureIncrease(nrPoints * 3 + nrCapTriangles * 3);
	const unsigned firstShell{ first + 1 };
	const unsigned firstCap{ firstShell + nrPoints };

	const float coneAngle{ cone.GetAngle() };
	const float coneCos{ cosf(coneAngle) };
	const float coneSin{ sinf(coneAngle) };

	const Float3 origin{ cone.origin + offset };
	const Float3 end{ cone.GetEnd() + offset };
	const Float3 right{ cone.GetRight() };
	const Float3 up{ cone.GetUp(right) };
	const float angleStep{ Constants::PI2 / Float::Cast(nrPoints) };

	vertices[first] = { origin, color, {} };

	for (unsigned i = 0; i < Float::Cast(nrPoints); i++)
	{
		const float angle{ i * angleStep };

		//shell
		Float3 pos{ right * cosf(angle) + up * sinf(angle) };
		const Float3 normal{ pos * coneCos - cone.direction * coneSin };
		pos *= cone.radius;
		pos += end;
		vertices[firstShell + i] = { pos, color, normal };

		//cap
		vertices[firstCap + i] = { pos, color, cone.direction };
	}

	for (unsigned i = 0; i < nrPoints; i++)
	{
		const unsigned idx1{ i + first + 1 };
		const unsigned idx2{ (i + 1 == nrPoints ? 0 : i + 1) + first + 1 };
		indices.Add(first, idx2, idx1);
	}

	MeshUtils::CapCircle(firstCap, nrPoints, indices);
}
