#pragma once

#include "..\MakerVertex.h"

namespace MyEngine
{
namespace MeshMaker2
{

template<typename TVertex>
class HoleArray
{
public:
	template<bool THasIndices>
	using Data = MakerDataBase<TVertex, THasIndices>;
	using MakerVertex = MakerVertex<TVertex>;

	HoleArray(
		unsigned nrHoles,
		unsigned nrHoleCorners, //min 5
		float radius, float gap);

	unsigned GetNrHoles() const { return m_NrHoles; }
	unsigned GetNrCorners() const { return m_NrCorners; }
	float GetRadius() const { return m_Radius; }
	float GetGap() const { return m_Gap; }

	float GetWidth() const { return m_Radius * 2; }
	float GetHeight() const { return m_Radius * 2 * m_NrHoles; }
	
	unsigned GetNrGaps() const { return m_NrHoles - 1; }

private:
	unsigned m_NrHoles;
	unsigned m_NrCorners; //per hole (sides of circle)
	float m_Radius;
	float m_Gap;
};
template<typename TVertex>
inline HoleArray<TVertex>::HoleArray(unsigned nrHoles, unsigned nrHoleCorners, float radius, float gap)
	: m_NrHoles{ nrHoles }
	, m_NrCorners{ nrHoleCorners }
	, m_Radius{ radius }
	, m_Gap{ gap }
{
}

}
}