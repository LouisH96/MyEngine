#include "Segment.h"

#include <iostream>
#include <vector>

#include "Intersection.h"
#include "Logger/Logger.h"
#include "Logger/ToString.h"
#include "Math/Vectors.h"

MyEngine::Io::Ttf::Segment::Segment(const Double2& begin, const Double2& controlPoint, const Double2& end)
	: m_Begin(begin)
	, m_End(end)
	, m_ControlPoint(controlPoint)
	, m_IsLinear(false)
{
}

MyEngine::Io::Ttf::Segment::Segment(const Double2& begin, const Double2& end)
	: m_Begin(begin)
	, m_End(end)
	, m_ControlPoint(INFINITY, 0)
	, m_IsLinear(true)
{
}

bool MyEngine::Io::Ttf::Segment::IsLinear() const
{
	return m_IsLinear;
}

void MyEngine::Io::Ttf::Segment::Translate(const Double2& translation)
{
	m_Begin += translation;
	m_End += translation;
	m_ControlPoint += translation;
}

void MyEngine::Io::Ttf::Segment::Scale(double scale)
{
	m_Begin *= scale;
	m_End *= scale;
	m_ControlPoint *= scale;
}

void MyEngine::Io::Ttf::Segment::AddIntersectionPoints(std::vector<Intersection>& intersections, double height) const
{
	if (IsLinear())
		AddIntersectionPointsLinear(intersections, height);
	else
		AddIntersectionPointsCurve(intersections, height);
}

void MyEngine::Io::Ttf::Segment::DebugPrint() const
{
	if (IsLinear())
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

void MyEngine::Io::Ttf::Segment::AddIntersectionPointsLinear(std::vector<Intersection>& intersections, double height) const
{
	const double divisor{ m_End.y - m_Begin.y };
	if (divisor == 0)
		return;
	const double result{ (height - m_Begin.y) / divisor };
	if (result >= 0 && result <= 1)
	{
		const Float2 point{ m_Begin + (m_End - m_Begin) * result };
		intersections.push_back(Intersection{ point.x, m_End.y > m_Begin.y });
	}
}

void MyEngine::Io::Ttf::Segment::AddIntersectionPointsCurve(std::vector<Intersection>& intersections, double height) const
{
	//https://www.wolframalpha.com/input?i=solve+t%2C+y+%3D+a+*+%281-t%29%5E2+%2B+b+*+2+*%281-t%29*t+%2B+c+*+t%5E2
	const double a{ m_Begin.y };
	const double b{ m_ControlPoint.y };
	const double c{ m_End.y };
	if (abs(a - (2 * b - c)) <= 0.001 && b != c)
	{
		const double result{ -(-2 * b + c + height) / (2 * (b - c)) };
		if (result >= 0 && result <= 1)
		{
			const Float2 point{ CalculatePoint(result) };
			intersections.push_back({ point.x, m_End.y > m_Begin.y });
		}
	}

	const double divisor{ a - 2 * b + c };
	if (divisor == 0)
		return;
	const double root{ sqrt(-a * c + a * height + b * b - 2 * b * height + c * height) };
	const double result1{ (root + a - b) / divisor };
	const double result2{ -(root - a + b) / divisor };
	if (result1 > 0 && result1 < 1)
	{
		const Float2 point{ CalculatePoint(result1) };
		intersections.push_back({ point.x, true });
	}
	if (result2 > 0 && result2 < 1)
	{
		const Float2 point{ CalculatePoint(result2) };
		intersections.push_back({ point.x, false });
	}
}

MyEngine::Double2 MyEngine::Io::Ttf::Segment::CalculatePoint(double alpha) const
{
	const double invAlpha = 1.0 - alpha;
	return m_Begin * (invAlpha * invAlpha)
		+ m_ControlPoint * (2 * invAlpha * alpha)
		+ m_End * (alpha * alpha);
}

void MyEngine::Io::Ttf::Segment::AddCurvePoints(Array<Double2>& points, int offset, int pointsPerCurve) const
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

void MyEngine::Io::Ttf::Segment::AddLinearPoints(Array<Double2>& points, int offset) const
{
	points[offset] = m_Begin;
	points[offset + 1] = m_End;
}
