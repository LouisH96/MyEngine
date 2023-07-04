#include "Glyph.h"

#include "DataStructures/Algorithms.h"
#include <algorithm>

MyEngine::Io::Ttf::Glyph::Glyph(const Array<Array<TtfPoint>>& ttfPoints, const Double2& minBounds, const Double2& maxBounds)
	: m_Contours{ ttfPoints.GetSize() }
	, m_MinBounds(minBounds)
	, m_MaxBounds(maxBounds)
{
	for (int i = 0; i < ttfPoints.GetSize(); i++)
		m_Contours[i] = Contour{ ttfPoints[i] };
}

MyEngine::Double2 MyEngine::Io::Ttf::Glyph::GetSize() const
{
	return m_MaxBounds - m_MinBounds;
}

void MyEngine::Io::Ttf::Glyph::Translate(const Double2& translation)
{
	m_MinBounds += translation;
	m_MaxBounds += translation;
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].Translate(translation);
}

void MyEngine::Io::Ttf::Glyph::Scale(double scale)
{
	m_MinBounds *= scale;
	m_MaxBounds *= scale;
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].Scale(scale);
}

void MyEngine::Io::Ttf::Glyph::Normalize()
{
	Translate(-m_MinBounds);
	const Double2 size{ GetSize() };
	const double scale{ size.x > size.y ? 1.0 / size.x : 1.0 / size.y };
	Scale(scale);
}

void MyEngine::Io::Ttf::Glyph::AddIntersections(std::vector<Intersection>& intersections, double height) const
{
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].AddIntersections(intersections, height);
	std::sort(intersections.begin(), intersections.end());
}

void MyEngine::Io::Ttf::Glyph::DebugDraw(const Float3& color, const Float3& offset, int pointsPerCurve) const
{
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].DebugDraw(color, offset, pointsPerCurve);
}

void MyEngine::Io::Ttf::Glyph::DebugDrawSegments(const Float3& offset, int pointsPerCurve) const
{
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].DebugDrawSegments(offset, pointsPerCurve);
}

void MyEngine::Io::Ttf::Glyph::DebugPrint()
{
	std::cout << "Glyph:\n";
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].DebugPrint();
}
