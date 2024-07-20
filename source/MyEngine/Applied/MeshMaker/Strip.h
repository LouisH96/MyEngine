#pragma once

#include <DataStructures\Pointers\SharedPtr.h>
#include <Rendering\Mesh\MeshData.h>

#include "MakerVertex.h"
#include "MeshMakerHelper.h"

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

	void AddEdge(Vertex bottom, Vertex top);
	void AddNormal(const Float3& normal);

	template<typename TVertex, ModelTopology Topology>
	void CalculateNormals_Sharp(const MeshData<TVertex, Topology>& meshData);

	const List<Edge>& GetEdges() const { return m_Edges; }
	List<Edge>& GetEdges() { return m_Edges; }

	const List<Float3>& GetNormals() const { return m_Normals; }
	List<Float3>& GetNormals() { return m_Normals; }

	const unsigned GetNrEdges() const { return m_Edges.GetSize(); }
	const unsigned GetNrWalls() const { return m_Edges.GetSize() - 1; }

private:
	List<Edge> m_Edges;
	List<Float3> m_Normals;
};
template<typename TVertex, ModelTopology Topology>
inline void Strip::CalculateNormals_Sharp(const MeshData<TVertex, Topology>& meshData)
{
	m_Normals.Clear();
	for (unsigned iWall{ 0 }; iWall < GetNrWalls(); iWall++)
	{
		const MakerVertex& leftBot{ m_Edges[iWall].Bottom.Get()};
		const MakerVertex& leftTop{ m_Edges[iWall].Top.Get() };
		const MakerVertex& rightBot{ m_Edges[iWall + 1].Bottom.Get() };

		const Float3 leftBotPos{ MeshMakerHelper::GetPosition(leftBot, meshData) };
		const Float3 leftTopPos{ MeshMakerHelper::GetPosition(leftTop, meshData) };
		const Float3 rightBotPos{ MeshMakerHelper::GetPosition(rightBot, meshData) };

		const Float3 toLeftTop{ leftTopPos - leftBotPos };
		const Float3 toRightBot{ rightBotPos - leftBotPos };

		m_Normals.Add(toLeftTop.Cross(toRightBot).Normalized());
	}
}
}
}