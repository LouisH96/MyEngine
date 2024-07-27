#pragma once

#include <DataStructures\Pointers\SharedPtr.h>
#include <Rendering\Mesh\MeshData.h>

#include "..\MakerVertex.h"
#include "..\MeshMakerHelper.h"
#include "Line.h"

namespace MyEngine
{
namespace MeshMaker
{
class Strip
{
public:
	using Vertex = SharedPtr<const MakerVertex>;

	Strip() = default;

	void EnsureEdgesSize(unsigned size);
	void EnsureNormalsSize(unsigned size);

	void AddEdge(Vertex bottom, Vertex top);
	void AddNormal(const Float3& normal);

	template<typename TVertex, ModelTopology Topology>
	void CalculateNormals_Sharp(const MeshData<TVertex, Topology>& meshData);

	const List<Line>& GetEdges() const { return m_Edges; }
	List<Line>& GetEdges() { return m_Edges; }

	const List<Float3>& GetNormals() const { return m_Normals; }
	List<Float3>& GetNormals() { return m_Normals; }

	const unsigned GetNrEdges() const { return m_Edges.GetSize(); }
	const unsigned GetNrWalls() const { return m_Edges.GetSize() - 1; }

	const Line& GetEdge(unsigned i) const;

	template<typename Adaptor>
	void AdaptEdges(Adaptor adaptor);

private:
	List<Line> m_Edges;
	List<Float3> m_Normals;
};
template<typename TVertex, ModelTopology Topology>
inline void Strip::CalculateNormals_Sharp(const MeshData<TVertex, Topology>& meshData)
{
	m_Normals.Clear();
	for (unsigned iWall{ 0 }; iWall < GetNrWalls(); iWall++)
	{
		const MakerVertex& leftBot{ m_Edges[iWall][0].Get() };
		const MakerVertex& leftTop{ m_Edges[iWall][1].Get() };
		const MakerVertex& rightBot{ m_Edges[iWall + 1][0].Get() };

		const Float3 leftBotPos{ MeshMakerHelper::GetPosition(leftBot, meshData) };
		const Float3 leftTopPos{ MeshMakerHelper::GetPosition(leftTop, meshData) };
		const Float3 rightBotPos{ MeshMakerHelper::GetPosition(rightBot, meshData) };

		const Float3 toLeftTop{ leftTopPos - leftBotPos };
		const Float3 toRightBot{ rightBotPos - leftBotPos };

		m_Normals.Add(toLeftTop.Cross(toRightBot).Normalized());
	}
}
template<typename Adaptor>
inline void Strip::AdaptEdges(Adaptor adaptor)
{
	for (unsigned iEdge{ 0 }; iEdge < m_Edges.GetSize(); ++iEdge)
		adaptor(m_Edges[iEdge]);
}
}
}