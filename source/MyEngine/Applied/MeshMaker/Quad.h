#pragma once

#include <DataStructures\Pointers\SharedPtr.h>

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
	Quad(SharedPtr<const MakerVertex> p0, SharedPtr<const MakerVertex> p1, SharedPtr<const MakerVertex> p2, SharedPtr<const MakerVertex> p3)
		: Vertices{ p0, p1, p2, p3 }
	{};

	SharedPtr<const MakerVertex> Vertices[NrVertices];
};
}
}