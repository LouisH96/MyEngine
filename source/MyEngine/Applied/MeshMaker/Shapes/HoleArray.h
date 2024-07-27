#pragma once

namespace MyEngine
{
namespace MeshMaker
{
class HoleArray
{
public:
	HoleArray(unsigned nrHoles, 
		float holeRadius, float holeGap, unsigned nrCornersPerHole );

	float GetHoleRadius() const { return m_HoleRadius; }
	float GetHoleGap() const { return m_HoleGap; }
	unsigned GetNrHoles() const { return m_NrHoles; }
	unsigned GetNrCornersPerHole() const { return m_NrCornersPerHole; }

	unsigned GetNrGaps() const { return m_NrHoles - 1; }

private:
	unsigned m_NrHoles;
	float m_HoleRadius;
	float m_HoleGap;
	unsigned m_NrCornersPerHole;
};
}
}