#pragma once

#include <DataStructures\Pointers\SharedPtr.h>

#include "..\MakerVertex.h"

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

	SharedPtr<const MakerVertex> GetLeftBot() const { return Vertices[0]; }
	SharedPtr<const MakerVertex> GetLeftTop() const { return Vertices[1]; }
	SharedPtr<const MakerVertex> GetRightBot() const { return Vertices[3]; }
	SharedPtr<const MakerVertex> GetRightTop() const { return Vertices[2]; }

	void SetLeftBot(SharedPtr<const MakerVertex> vertex) { Vertices[0] = vertex; }
	void SetLeftTop(SharedPtr<const MakerVertex> vertex) { Vertices[1] = vertex; }
	void SetRightBot(SharedPtr<const MakerVertex> vertex) { Vertices[3] = vertex; }
	void SetRightTop(SharedPtr<const MakerVertex> vertex) { Vertices[2] = vertex; }

	SharedPtr<const MakerVertex> Vertices[NrVertices];
};
}
}