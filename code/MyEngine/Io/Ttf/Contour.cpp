#include "pch.h"
#include "Contour.h"

#include "ContourOperations.h"
#include "Debug/DebugRenderer.h"

Io::Ttf::Contour::Contour(const Array<TtfPoint>& ttfPoints)
	: m_Segments(ContourOperations::ToSegments(ttfPoints))
{
}

void Io::Ttf::Contour::Translate(const Math::Double2& translation)
{
	for (int i = 0; i < m_Segments.GetSize(); i++)
		m_Segments[i].Translate(translation);
}

void Io::Ttf::Contour::Scale(double scale)
{
	for (int i = 0; i < m_Segments.GetSize(); i++)
		m_Segments[i].Scale(scale);
}

void Io::Ttf::Contour::AddIntersections(std::vector<Intersection>& intersections, double height) const
{
	for (int i = 0; i < m_Segments.GetSize(); i++)
		m_Segments[i].AddIntersectionPoints(intersections, height);
}

void Io::Ttf::Contour::DebugDraw(const Math::Float3& color, const Math::Float3& offset, int pointsPerCurve) const
{
	int nrPoints = 1;
	for (int i = 0; i < m_Segments.GetSize(); i++)
		nrPoints += m_Segments[i].IsLinear() ? 1 : pointsPerCurve - 1;

	Array<Math::Double2> points{ nrPoints };
	int iPoint = 0;
	for (int iSegment = 0; iSegment < m_Segments.GetSize(); iSegment++)
	{
		if (m_Segments[iSegment].IsLinear())
		{
			m_Segments[iSegment].AddLinearPoints(points, iPoint);
			iPoint++;
		}
		else
		{
			m_Segments[iSegment].AddCurvePoints(points, iPoint, pointsPerCurve);
			iPoint += pointsPerCurve - 1;
		}
	}
	//points.Last() = points.First();

	Array<Math::Float3> drawPoints{ points.GetSize() };
	for (int i = 0; i < drawPoints.GetSize(); i++)
		drawPoints[i] = Math::Float3{ static_cast<float>(points[i].x) + offset.x, static_cast<float>(points[i].y) + offset.y, offset.z };
	DebugRenderer::AddLine(drawPoints, color);
}

void Io::Ttf::Contour::DebugPrint()
{
	std::cout << "Contour:\n";
	for (int i = 0; i < m_Segments.GetSize(); i++)
		m_Segments[i].DebugPrint();
}
