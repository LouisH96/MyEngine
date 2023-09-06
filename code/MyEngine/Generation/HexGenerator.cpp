#include "pch.h"
#include "HexGenerator.h"

#include "Geometry/Shapes/Hexagon.h"

using namespace Generation;
using namespace Rendering;

void HexGenerator::Generate(
	const Hexagon& hex,
	const Float3& right, const Float3& normal,
	List<V_PosNor>& vertices, List<int>& indices)
{
	const unsigned firstIdx{ vertices.GetSizeU() };
	vertices.EnsureIncrease(Hexagon::NR_POINTS);
	indices.EnsureIncrease(4 * 3);
	const Float3 up{ (right.Cross(normal).Normalized()) };

	//vertices
	constexpr float angleStep{ Constants::TO_RAD * 360.f / Hexagon::NR_POINTS };
	for (unsigned i = 0; i < Hexagon::NR_POINTS; i++)
	{
		const float angle{ angleStep * i };
		vertices.Add({ (right * cosf(angle) + up * sinf(angle)) * hex.GetOuterRadius(), normal });
	}

	//indices
	indices.Add(0 + firstIdx, 2 + firstIdx, 1 + firstIdx);
	indices.Add(0 + firstIdx, 3 + firstIdx, 2 + firstIdx);
	indices.Add(0 + firstIdx, 5 + firstIdx, 3 + firstIdx);
	indices.Add(3 + firstIdx, 5 + firstIdx, 4 + firstIdx);
}

void HexGenerator::GenerateFlatRight(const Hexagon& hex, const Float3& right, const Float3& normal,
	List<Rendering::V_PosNor>& vertices, List<int>& indices)
{
	const unsigned firstIdx{ vertices.GetSizeU() };
	vertices.EnsureIncrease(Hexagon::NR_POINTS);
	indices.EnsureIncrease(4 * 3);
	const Float3 up{ (right.Cross(normal).Normalized()) };

	//vertices
	constexpr float angleStep{ Constants::TO_RAD * 360.f / Hexagon::NR_POINTS };
	for (unsigned i = 0; i < Hexagon::NR_POINTS; i++)
	{
		const float angle{ angleStep * (static_cast<float>(i) - .5f) };
		vertices.Add({ (right * cosf(angle) + up * sinf(angle)) * hex.GetOuterRadius(), normal });
	}

	//indices
	indices.Add(0 + firstIdx, 2 + firstIdx, 1 + firstIdx);
	indices.Add(0 + firstIdx, 3 + firstIdx, 2 + firstIdx);
	indices.Add(0 + firstIdx, 5 + firstIdx, 3 + firstIdx);
	indices.Add(3 + firstIdx, 5 + firstIdx, 4 + firstIdx);
}
