#pragma once

#include <DataStructures\Array.h>
#include "..\MakerResult.h"

namespace MyEngine
{
namespace MeshMaker
{
class StripResult
	: public MakerResult
{
public:
	void SetNrEdges(unsigned nrEdges);

	void SetPoint(unsigned iEdge, unsigned iEdgeEnd, unsigned vertexIndex);
	void SetEdgeBot(unsigned iEdge, unsigned vertexIndex);
	void SetEdgeTop(unsigned iEdge, unsigned vertexIndex);

	unsigned GetPoint(unsigned iEdge, unsigned iEdgeEnd);
	unsigned GetEdgeBot(unsigned iEdge);
	unsigned GetEdgeTop(unsigned iEdge);

private:
	Array<unsigned> m_Vertices;
};

class DefaultStripResult
	: public MakerResult
{
public:
	void SetNrEdges(unsigned) {};
	void SetPoint(unsigned, unsigned, unsigned) {};
	void SetEdgeBot(unsigned, unsigned) {};
	void SetEdgeTop(unsigned, unsigned) {};
};
}
}