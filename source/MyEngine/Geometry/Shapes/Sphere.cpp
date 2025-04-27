#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(const Float3& center, float radius)
	: m_Center{ center }
	, m_Radius{ radius }
{
}

void Sphere::SetCenter(const Float3& center)
{
	m_Center = center;
}

void Sphere::SetRadius(float radius)
{
	m_Radius = radius;
}

Sphere Sphere::operator+(const Float3& movement) const
{
	return Sphere{ m_Center + movement, m_Radius };
}

Sphere Sphere::operator-(const Float3& movement) const
{
	return Sphere{ m_Center - movement, m_Radius };
}

CubeAA Sphere::GetGlobalBounds(const Float3& direction, float distance) const
{
	return Sphere::GetGlobalBounds(
		m_Center, m_Radius,
		direction, distance);
}

CubeAA Sphere::GetGlobalBounds(
	const Float3& center, float radius,
	const Float3& direction, float distance)
{
	Float3 minBounds{ center - Float3{radius} };
	Float3 maxBounds{ center + Float3{radius} };

	for (unsigned iAxis{ 0 }; iAxis < 3; ++iAxis)
	{
		if (direction[iAxis] < 0)
			minBounds[iAxis] += direction[iAxis] * distance;
		else
			maxBounds[iAxis] += direction[iAxis] * distance;
	}

	return CubeAA{ minBounds, maxBounds - minBounds };
}
