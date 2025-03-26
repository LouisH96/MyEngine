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
