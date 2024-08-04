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
class FlatStrip
{
public:
	using Vertex = SharedPtr<const MakerVertex>;

	FlatStrip() = default;

	void EnsureEdgesSize(unsigned size);

	void AddEdge(Vertex bottom, Vertex top);
	void SetNormal(const Float3& normal);

	void SetEdgeBot(unsigned iEdge, Vertex vertex);
	void SetEdgeTop(unsigned iEdge, Vertex vertex);

	const List<Line>& GetEdges() const { return m_Edges; }
	List<Line>& GetEdges() { return m_Edges; }

	const Float3& GetNormal() const { return m_Normal; }

	const unsigned GetNrEdges() const { return m_Edges.GetSize(); }
	const unsigned GetNrWalls() const { return GetNrEdges() - 1; }

	const Line& GetEdge(unsigned i) const;

	template<typename TVertex, ModelTopology TTopology>
	void MoveEdges(const Float3& movement, MeshData<TVertex, TTopology>& meshData);

private:
	List<Line> m_Edges;
	Float3 m_Normal;
};

template<typename TVertex, ModelTopology TTopology>
inline void FlatStrip::MoveEdges(const Float3& movement, MeshData<TVertex, TTopology>& meshData)
{
	for (unsigned iEdge{ 0 }; iEdge < m_Edges.GetSize(); ++iEdge)
	{
		Line& line{ m_Edges[iEdge] };

		for (unsigned iVertex{ 0 }; iVertex < Line::NrVertices; ++iVertex)
		{
			SharedPtr<const MakerVertex>& lineEnd{ line[iVertex] };
			const Float3 oldPos{ MeshMakerHelper::GetPosition(lineEnd.Get(), meshData)};
			lineEnd = MakerPointVertex{ oldPos + movement };
		}
	}
}
}
}