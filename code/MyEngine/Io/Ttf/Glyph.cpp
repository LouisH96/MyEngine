#include "pch.h"
#include "Glyph.h"

Io::Ttf::Glyph::Glyph(const Array<Array<TtfPoint>>& ttfPoints, const Math::Double2& minBounds, const Math::Double2& maxBounds)
	: m_Contours{ ttfPoints.GetSize() }
	, m_MinBounds(minBounds)
	, m_MaxBounds(maxBounds)
{
	for (int i = 0; i < ttfPoints.GetSize(); i++)
		m_Contours[i] = Contour{ ttfPoints[i] };
}

Math::Double2 Io::Ttf::Glyph::GetSize() const
{
	return m_MaxBounds - m_MinBounds;
}

void Io::Ttf::Glyph::Translate(const Math::Double2& translation)
{
	m_MinBounds += translation;
	m_MaxBounds += translation;
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].Translate(translation);
}

void Io::Ttf::Glyph::Scale(double scale)
{
	m_MinBounds *= scale;
	m_MaxBounds *= scale;
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].Scale(scale);
}

void Io::Ttf::Glyph::Normalize()
{
	Translate(-m_MinBounds);
	const Math::Double2 size{ GetSize() };
	const double scale{ size.x > size.y ? 1.0 / size.x : 1.0 / size.y };
	Scale(scale);
}

void Io::Ttf::Glyph::AddIntersections(std::vector<Intersection>& intersections, double height) const
{
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].AddIntersections(intersections, height);
}

void Io::Ttf::Glyph::DebugDraw(const Math::Float3& color, const Math::Float3& offset, int pointsPerCurve) const
{
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].DebugDraw(color, offset, pointsPerCurve);
}

void Io::Ttf::Glyph::DebugPrint()
{
	std::cout << "Glyph:\n";
	for (int i = 0; i < m_Contours.GetSize(); i++)
		m_Contours[i].DebugPrint();
}
