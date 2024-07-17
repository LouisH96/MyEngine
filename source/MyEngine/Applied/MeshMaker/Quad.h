#pragma once

#include "MakerVertex.h"

namespace MyEngine
{
namespace MeshMaker
{
class Quad
{
public:
	static constexpr unsigned NrVertices{ 4 };

	Quad() = default;
	Quad(const MakerVertex& p0, const MakerVertex& p1, const MakerVertex& p2, const MakerVertex& p3)
		: pVertices{ &p0, &p1, &p2, &p3 }
	{};

	const MakerVertex* pVertices[NrVertices];
};
}
}