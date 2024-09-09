#pragma once

#include <Geometry\ModelTopology.h>

namespace MyEngine
{
namespace MeshMaker2
{

template<typename TVertex, ModelTopology TTopology>
class EmptyStripResult
	: public MakerResult<TVertex, TTopology>
{
public:
	void SetNrEdges(unsigned nrEdges) {};
	void AddEdge(unsigned, int, int) {};
	void AddEdgeBot(unsigned, int) {};
	void AddEdgeTop(unsigned, int) {};
};

template<typename TVertex, ModelTopology TTopology>
class StripResult
	: public MakerResult<TVertex, TTopology>
{
public:
	StripResult() {};
	
	void SetNrEdges(unsigned nrEdges) {
		m_Indices = { nrEdges * 2 };
	}

	void AddEdge(unsigned iEdge, int botIndex, int topIndex) {
		m_Indices[iEdge * 2] = botIndex;
		m_Indices[iEdge * 2 + 1] = topIndex;
	}
	void AddEdgeBot(unsigned iEdge, int index) {
		m_Indices[iEdge * 2] = index;
	}
	void AddEdgeTop(unsigned iEdge, int index) {
		m_Indices[iEdge * 2 + 1] = index;
	}

	int GetEdgeBot(unsigned iEdge) const { return m_Indices[iEdge * 2]; }
	int GetEdgeTop(unsigned iEdge) const { return m_Indices[iEdge * 2 + 1]; }

private:
	Array<int> m_Indices{};

};
}
}