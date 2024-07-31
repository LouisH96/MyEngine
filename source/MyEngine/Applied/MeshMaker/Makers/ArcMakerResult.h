#pragma once

#include <DataStructures\List.h>
#include "..\MakerResult.h"

namespace MyEngine
{
namespace MeshMaker
{
#define TEMP_DEF template<typename TVertex, ModelTopology TTopology>
#define TEMP_ARG TVertex, TTopology

template<typename TVertex, ModelTopology TTopology>
class ArcMakerResult
	: public MakerResult<TVertex, TTopology>
{
public:
	void SetNrCorners(unsigned nrCorners);

	void SetCenter(unsigned index);
	void SetCorner(unsigned iCorner, unsigned index);

	unsigned GetCenter() const { return m_ArcIndices[0]; }
	unsigned GetCorner(unsigned iCorner) const { return m_ArcIndices[1 + iCorner]; }

	unsigned GetFirstCorner() const { return m_ArcIndices[1]; }
	unsigned GetLastCorner() const { return m_ArcIndices.Last(); }

private:
	//just one of each of 'type', some topologies have multiple of each

	Array<unsigned> m_ArcIndices; //first is (a) center, others is one of each corner
};

TEMP_DEF
inline void ArcMakerResult<TEMP_ARG>::SetNrCorners(unsigned nrCorners)
{
	m_ArcIndices = { nrCorners + 1 };
}

TEMP_DEF
inline void ArcMakerResult<TEMP_ARG>::SetCenter(unsigned index)
{
	m_ArcIndices[0] = index;
}

TEMP_DEF
inline void ArcMakerResult<TEMP_ARG>::SetCorner(unsigned iCorner, unsigned index)
{
	m_ArcIndices[1 + iCorner] = index;
}

#undef TEMP_ARG
#undef TEMP_DEF

template<typename TVertex, ModelTopology TTopology>
class DefaultArcMakerResult
	: public MakerResult<TVertex, TTopology>
{
public:
	void SetNrCorners(unsigned) {};

	void SetCenter(unsigned) {};
	void SetCorner(unsigned, unsigned) {};
};

}
}