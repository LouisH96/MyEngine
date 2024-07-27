#pragma once

#include <DataStructures\Pointers\SharedPtr.h>
#include "..\MakerVertex.h"

namespace MyEngine
{
namespace MeshMaker
{
struct Line
{
	using Vertex = SharedPtr<const MakerVertex>;
	static constexpr unsigned NrVertices{ 2 };

	Vertex& operator[](unsigned idx) { return Vertices[idx]; }
	const Vertex& operator[](unsigned idx) const { return Vertices[idx]; }

	Vertex Vertices[NrVertices];
};
}
}