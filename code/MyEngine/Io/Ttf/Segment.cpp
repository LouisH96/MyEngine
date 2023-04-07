#include "pch.h"
#include "Segment.h"

Io::Ttf::Segment::Segment(const Math::Double2& begin, const Math::Double2& controlPoint, const Math::Double2& end)
	: m_Begin(begin)
	, m_End(end)
	, m_ControlPoint(controlPoint)
	, m_IsLinear(false)
{
}

Io::Ttf::Segment::Segment(const Math::Double2& begin, const Math::Double2& end)
	: m_Begin(begin)
	, m_End(end)
	, m_ControlPoint(INFINITY, 0)
	, m_IsLinear(true)
{
}

bool Io::Ttf::Segment::IsLinear() const
{
	return m_IsLinear;
}

void Io::Ttf::Segment::Translate(const Math::Double2& translation)
{
	m_Begin += translation;
	m_End += translation;
	m_ControlPoint += translation;
}

void Io::Ttf::Segment::Scale(double scale)
{
	m_Begin *= scale;
	m_End *= scale;
	m_ControlPoint *= scale;
}

void Io::Ttf::Segment::DebugPrint() const
{
	if(IsLinear())
	{
		std::cout << "LinearSegment: \n";
		std::cout << "\t Begin: " << ToString::Convert(m_Begin) << std::endl;
		std::cout << "\t End:" << ToString::Convert(m_End) << std::endl;
		return;
	}
	std::cout << "CurveSegment: \n";
	std::cout << "\t Begin: " << ToString::Convert(m_Begin) << std::endl;
	std::cout << "\t ControlPoint: " << ToString::Convert(m_ControlPoint) << std::endl;
	std::cout << "\t End: " << ToString::Convert(m_End) << std::endl;
}

Math::Double2 Io::Ttf::Segment::CalculatePoint(double alpha) const
{
	const double invAlpha = 1.0 - alpha;
	return m_Begin * (invAlpha * invAlpha)
		+ m_ControlPoint * (2 * invAlpha * alpha)
		+ m_End * (alpha * alpha);
}

void Io::Ttf::Segment::AddCurvePoints(Array<Math::Double2>& points, int offset, int pointsPerCurve) const
{
	if (pointsPerCurve < 2)
	{
		Logger::PrintError("Need at least 2 points to draw curve");
		return;
	}
	const double alphaStep = 1.0 / (pointsPerCurve - 1);
	for (int i = 0; i < pointsPerCurve; i++)
		points[offset++] = CalculatePoint(i * alphaStep);
}

void Io::Ttf::Segment::AddLinearPoints(Array<Math::Double2>& points, int offset) const
{
	points[offset] = m_Begin;
	points[offset + 1] = m_End;
}
