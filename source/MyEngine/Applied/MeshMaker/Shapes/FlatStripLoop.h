#pragma once

#include "Line.h"

namespace MyEngine
{
namespace MeshMaker
{
class FlatStripLoop
{
public:
	using Vertex = SharedPtr<const MakerVertex>;

	FlatStripLoop() = default;
	FlatStripLoop(unsigned capacity);

	void AddEdge(Vertex bottom, Vertex top);
	void AddEdge(Line line);
	void SetNormal(const Float3& normal);

	void SetEdges(const List<Line>& edges);

	const List<Line>& GetEdges() const { return m_Edges; }
	List<Line>& GetEdges() { return m_Edges; }

	const Line& GetEdge(unsigned iEdge) const { return m_Edges[iEdge % m_Edges.GetSize()]; }
	Line& GetEdge(unsigned iEdge) { return m_Edges[iEdge % m_Edges.GetSize()]; }

	const Float3& GetNormal() const { return m_Normal; }
	const Float3& GetNormal(unsigned) const { return m_Normal; }

	const unsigned GetNrEdges() const { return m_Edges.GetSize(); }
	const unsigned GetNrWalls() const { return m_Edges.GetSize(); }

private:
	List<Line> m_Edges;
	Float3 m_Normal;
};
}
}