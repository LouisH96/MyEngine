#pragma once

#include <DataStructures\List.h>
#include "..\MakerResult.h"

namespace MyEngine
{
namespace MeshMaker
{
class ArcMakerResult
	: public MakerResult
{
public:
	void SetNrCorners(unsigned nrCorners);

	void SetCenter(unsigned index);
	void SetCorner(unsigned iCorner, unsigned index);

	unsigned GetCenter() const { return m_ArcIndices[0]; }
	unsigned GetCorner(unsigned iCorner) const { return m_ArcIndices[1 + iCorner]; }

	unsigned GetLastCorner() const { return m_ArcIndices.Last(); }

private:
	//just one of each of 'type', some topologies have multiple of each

	Array<unsigned> m_ArcIndices; //first is (a) center, others is one of each corner
};

class DefaultArcMakerResult
	: public MakerResult
{
public:
	void SetNrCorners(unsigned) {};

	void SetCenter(unsigned) {};
	void SetCorner(unsigned, unsigned) {};
};

}
}