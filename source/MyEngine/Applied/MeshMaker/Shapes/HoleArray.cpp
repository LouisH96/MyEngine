#include "pch.h"
#include "HoleArray.h"

using namespace MeshMaker;

HoleArray::HoleArray(unsigned nrHoles,
	float holeRadius, float holeGap, unsigned nrCornersPerHole)
	: m_NrHoles{ nrHoles }
	, m_HoleRadius{ holeRadius }
	, m_HoleGap{ holeGap }
	, m_NrCornersPerHole{ nrCornersPerHole }
{
}
