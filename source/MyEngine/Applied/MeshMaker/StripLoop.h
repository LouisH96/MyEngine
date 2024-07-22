#pragma once

#include <DataStructures\Pointers\SharedPtr.h>
#include <Rendering\Mesh\MeshData.h>
#include "MakerVertex.h"
#include "Line.h"
#include "MeshMakerHelper.h"

namespace MyEngine
{
namespace MeshMaker
{
class StripLoop
{
public:
	using Vertex = SharedPtr<const MakerVertex>;

	StripLoop() = default;

	void AddEdge(Vertex bottom, Vertex top);
	void AddEdge(Line line);
	void AddNormal(const Float3& normal);

	void SetEdges(const List<Line>& edges);
	void SetNormals(const List<Float3>& normals);

	template<typename TVertex, ModelTopology TTopology>
	void CalculateMissingNormals_Sharp(const MeshData<TVertex, TTopology>& meshData);

	const List<Line>& GetEdges() const { return m_Edges; }
	List<Line>& GetEdges() { return m_Edges; }

	const List<Float3>& GetNormals() const { return m_Normals; }
	List<Float3>& GetNormals() { return m_Normals; }

	const unsigned GetNrEdges() const { return m_Edges.GetSize(); }
	const unsigned GetNrWalls() const { return m_Edges.GetSize(); }

	const Line& GetEdge(unsigned i) const;

private:
	List<Line> m_Edges;
	List<Float3> m_Normals;
};
template<typename TVertex, ModelTopology TTopology>
inline void StripLoop::CalculateMissingNormals_Sharp(const MeshData<TVertex, TTopology>& meshData)
{
	m_Normals.EnsureCapacity(GetNrWalls());
	for (unsigned iWall{ m_Normals.GetSize() }; iWall < GetNrWalls() - 1; iWall++)
	{
		const Line& left{ m_Edges[iWall] };
		const Line& right{ m_Edges[iWall + 1] };
		m_Normals.Add(MeshMakerHelper::GetNormal(left, right, meshData));
	}
	//last
	const Line& lastLeft{ m_Edges.Last() };
	const Line& lastRight{ m_Edges.First() };
	m_Normals.Add(MeshMakerHelper::GetNormal(lastLeft, lastRight, meshData));
}
}
}