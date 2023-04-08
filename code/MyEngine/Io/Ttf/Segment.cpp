#include "pch.h"
#include "Segment.h"

#include "Math/Float2.h"
#include "Intersection.h"

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

void Io::Ttf::Segment::AddIntersectionPoints(std::vector<Intersection>& intersections, double height) const
{
	if (IsLinear())
		AddIntersectionPointsLinear(intersections, height);
	else
		AddIntersectionPointsCurve(intersections, height);
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

void Io::Ttf::Segment::AddIntersectionPointsLinear(std::vector<Intersection>& intersections, double height) const
{
	const double divisor{ m_End.y - m_Begin.y };
	if (divisor == 0)
		return;
	const double result{ (height - m_Begin.y) / divisor };
	if (result >= 0 && result <= 1)
	{
		const Math::Float2 point{ m_Begin + (m_End - m_Begin) * result };
		intersections.push_back(Intersection{ point.x, m_End.y > m_Begin.y });
	}
}

void Io::Ttf::Segment::AddIntersectionPointsCurve(std::vector<Intersection>& intersections, double height) const
{
	//https://www.wolframalpha.com/input?i=solve+t%2C+y+%3D+a+*+%281-t%29%5E2+%2B+b+*+2+*%281-t%29*t+%2B+c+*+t%5E2
	const double a{ m_Begin.y };
	const double b{m_ControlPoint.y };
	const double c{ m_End.y };
	const double divisor{ a - 2 * b + c };
	if (divisor == 0)
		return;
	const double sqrt{ sqrtf(-a * c + a * height + b * b - 2 * b * height + c * height) };
	const double result1{ (sqrt + a - b) / divisor };
	const double result2{ -(sqrt - a + b) / divisor };
	if (result1 >= 0 && result1 <= 1)
	{
		const Math::Float2 point{ CalculatePoint(result1) };
		intersections.push_back({ point.x, true });
	}
	if (result2 >= 0 && result2 <= 1)
	{
		const Math::Float2 point{ CalculatePoint(result2) };
		intersections.push_back({ point.x, false });
	}
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
