#pragma once

#include <DataStructures\Pointers\SharedPtr.h>

#include "MakerVertex.h"

namespace MyEngine
{
namespace MeshMaker
{
class Strip
{
public:
	using Vertex = SharedPtr<const MakerVertex>;

	struct Edge {
		Vertex Bottom;
		Vertex Top;
	};

	Strip() = default;

	void Add(Vertex bottom, Vertex top);
	void AddNormal(const Float3& normal);

	const List<Edge>& GetEdges() const { return m_Edges; }
	List<Edge>& GetEdges() { return m_Edges; }

	const List<Float3>& GetNormals() const { return m_Normals; }
	List<Float3>& GetNormals() { return m_Normals; }

private:
	List<Edge> m_Edges;
	List<Float3> m_Normals;
};
}
}