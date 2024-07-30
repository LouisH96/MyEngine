#include "pch.h"
#include "ArcMakerResult.h"

using namespace MeshMaker;

void ArcMakerResult::SetNrCorners(unsigned nrCorners)
{
	m_ArcIndices = { nrCorners + 1 };
}

void ArcMakerResult::SetCenter(unsigned index)
{
	m_ArcIndices[0] = index;
}

void ArcMakerResult::SetCorner(unsigned iCorner, unsigned index)
{
	m_ArcIndices[1 + iCorner] = index;
}
